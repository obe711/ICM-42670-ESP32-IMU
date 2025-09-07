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
#define LOG_LEVEL_LOCAL ESP_LOG_NONE

static uint8_t man_log_set = 0;

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

/**
 * @brief Calculate the angle of tilt
 *
 * @param x X axis value
 * @param y Y axis value
 * @param z Z axis value
 * @return Measurement of inclination (Degree)
 */
float calculate_tilt_angle(float x, float y, float z) {
  return fabsf(atan2f(x, sqrtf(y * y + z * z)) * RAD_TO_DEG);
}

/**
 * @brief Set the log output level
 *
 * @param log_level
 * @return
 *      - ESP_OK                Set level success
 */
esp_err_t set_icm42670_log_level(esp_log_level_t log_level) {
  man_log_set = 1;
  esp_log_level_set("ICM42670", log_level);
  esp_log_level_set(TAG, log_level);

  return ESP_OK;
};

/**
 * @brief   Install and start ICM 42670 driver.
 * @note    This function must be called before any ICM 42670 driver read
 *          operations.
 *
 *
 * @param   esp_config      Pointer to config - see icm42670_esp_config_t struct
 *
 * @return
 *     - ESP_OK              Success
 *     - ESP_FAIL            Init error
 */
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

  if (!man_log_set) {
    esp_log_level_set("ICM42670", LOG_LEVEL_LOCAL);
    esp_log_level_set(TAG, LOG_LEVEL_LOCAL);
  }

  return ESP_OK;
}
