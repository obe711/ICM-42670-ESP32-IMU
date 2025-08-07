/**
 * @file icm42670-esp32.c
 * @author Obediah Klopfenstein (obe711@gmail.com)
 * @brief ICM-42670 driver
 * @version 1
 * @date 2024-11-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "icm42670-esp32.h"
#include <math.h>

#define TAG "icm42670-esp32"

#define ICM_ADDRESS 0x68

#define TILT_THRESHOLD 60.0f    // degrees
#define RETURN_THRESHOLD 15.0f  // degrees
#define RAD_TO_DEG 57.27272727f // 57.295779513f

// static i2c_port_t i2c_master_port = I2C_MASTER_NUM;

// static icm42670_handle_t icm_sensor;

// icm42670_cfg_t sensor_cfg = {
//     ACCE_FS_8G,      /*!< Accelerometer full scale range */
//     ACCE_ODR_1600HZ, /*!< Accelerometer ODR selection */
//     GYRO_FS_2000DPS, /*!< Gyroscope full scale range */
//     GYRO_ODR_1600HZ, /*!< Gyroscope ODR selection */
// };

float calculate_tilt_angle(float x, float y, float z) {
  return fabsf(atan2f(x, sqrtf(y * y + z * z)) * RAD_TO_DEG);
}

// static void imu_task(void *pvParameter) {

//   // float sensitivity;

//   // ret = icm42670_get_acce_sensitivity(icm_sensor, &sensitivity);
//   // if (ret != ESP_OK)
//   // {
//   //   ESP_LOGE(TAG, "icm42670_get_acce_sensitivity");
//   // }
//   // ESP_LOGI(TAG, "acce_sensitivity: %.2f", sensitivity);

//   // ret = icm42670_get_gyro_sensitivity(icm_sensor, &sensitivity);
//   // if (ret != ESP_OK)
//   // {
//   //   ESP_LOGE(TAG, "icm42670_get_gyro_sensitivity");
//   // }
//   // ESP_LOGI(TAG, "gyro_sensitivity: %.2f", sensitivity);

//   icm42670_value_t acce_value = {0};
//   icm42670_value_t gyro_value = {0};
//   complimentary_angle_t comp_angle;

//   float tilt_angle;
//   bool is_tilted = false;

//   if (icm42670_get_acce_value(icm_sensor, &acce_value) != ESP_OK) {
//     ESP_LOGE(TAG, "icm42670_get_acce_value");
//   };

//   if (icm42670_get_gyro_value(icm_sensor, &gyro_value) != ESP_OK) {
//     ESP_LOGE(TAG, "icm42670_get_gyro_value");
//   };

//   // if (icm42670_complimentory_filter(icm_sensor, &acce_value, &gyro_value,
//   // &comp_angle) != ESP_OK)
//   // {
//   //   ESP_LOGE(TAG, "icm42670_complimentory_filter");
//   // };

//   while (1) {

//     if (icm42670_get_acce_value(icm_sensor, &acce_value) != ESP_OK) {
//       ESP_LOGE(TAG, "icm42670_get_acce_value");
//     };

//     if (icm42670_get_gyro_value(icm_sensor, &gyro_value) != ESP_OK) {
//       ESP_LOGE(TAG, "icm42670_get_gyro_value");
//     };

//     // if (icm42670_complimentory_filter(icm_sensor, &acce_value,
//     &gyro_value,
//     // &comp_angle) != ESP_OK)
//     // {
//     //   ESP_LOGE(TAG, "icm42670_complimentory_filter");
//     // };

//     tilt_angle = calculate_tilt_angle(acce_value.x, acce_value.y,
//     acce_value.z); if (!is_tilted && tilt_angle > TILT_THRESHOLD) {
//       printf("Tilt detected! Angle: %.2f degrees\n", tilt_angle);
//       is_tilted = true;
//     } else if (is_tilted && tilt_angle < RETURN_THRESHOLD) {
//       printf("Object returned to position. Angle: %.2f degrees\n",
//       tilt_angle); is_tilted = false;
//     }
//     ESP_LOGD(TAG, "Tilt angle: %.2f", tilt_angle);

//     // ESP_LOGD(TAG, "acce: X = %.2f, Y = %.2f, Z = %.2f", acce_value.x,
//     // acce_value.y, acce_value.z); ESP_LOGD(TAG, "gyro: X = %.2f, Y = %.2f,
//     Z =
//     // %.2f", gyro_value.x, gyro_value.y, gyro_value.z); ESP_LOGD(TAG, "comp:
//     // pitch = %.2f, roll = %.2f \n", comp_angle.pitch, comp_angle.roll);

//     // ESP_LOGD(TAG, "Y: %.2f - %.2f", gyro_value.y, acce_value.y);

//     // ESP_LOGI(TAG, "initial_pitch: %.2f, current_pitch: %.2f, delta: %.2f",
//     // initial_pitch, current_pitch, delta_pitch);

//     vTaskDelay(pdMS_TO_TICKS(100));
//   }
// }

esp_err_t init_icm42670(icm42670_esp_config_t *esp_config) {
  ESP_LOGI(TAG, "Init icm42670");

  esp_config->icm_sensor =
      icm42670_create(esp_config->i2c_master_port, ICM_ADDRESS);
  if (esp_config->icm_sensor == NULL) {
    ESP_LOGE(TAG, "icm42670_create fail");
    return ESP_FAIL;
  }

  if (icm42670_config(esp_config->icm_sensor, esp_config->sensor_cfg) !=
      ESP_OK) {
    ESP_LOGE(TAG, "Config fail");
    return ESP_FAIL;
  };

  if (icm42670_acce_set_pwr(esp_config->icm_sensor, esp_config->acce_pwr) !=
      ESP_OK) {
    ESP_LOGE(TAG, "icm42670_acce_set_pwr");
    return ESP_FAIL;
  }

  if (icm42670_gyro_set_pwr(esp_config->icm_sensor, esp_config->gyro_pwr) !=
      ESP_OK) {
    ESP_LOGE(TAG, "icm42670_gyro_set_pwr");
    return ESP_FAIL;
  }

  // vTaskDelay(pdMS_TO_TICKS(20));

  // xTaskCreate(imu_task, "imu_task", 4095, NULL, 5, NULL);

  return ESP_OK;
}
