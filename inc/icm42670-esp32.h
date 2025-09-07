/**
 * @file icm42670-esp32.h
 * @author Obediah Klopfenstein (obe711@gmail.com)
 * @brief Main header for ICM-42670 driver
 * @version 1
 * @date 2024-11-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "driver/i2c.h"
#include "esp_log.h"
#include "icm42670.h"
#include <stdint.h>
#include <string.h>

typedef struct {
  i2c_port_t i2c_master_port;    /*!< I2C port number*/
  icm42670_acce_pwr_t acce_pwr;  /*!< see icm42670_acce_pwr_t*/
  icm42670_gyro_pwr_t gyro_pwr;  /*!< see icm42670_gyro_pwr_t*/
  icm42670_handle_t *icm_sensor; /*!< Pointer to your icm_sensor handle*/
  icm42670_cfg_t *sensor_cfg;    /*!< Pointer to your sensor_cfg*/
} icm42670_esp_config_t;

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
esp_err_t init_icm42670(icm42670_esp_config_t *esp_config);

/**
 * @brief Calculate the angle of tilt
 *
 * @param x X axis value
 * @param y Y axis value
 * @param z Z axis value
 * @return Measurement of inclination (Degree)
 */
float calculate_tilt_angle(float x, float y, float z);

/**
 * @brief Set the log output level
 *
 * @param log_level
 * @return
 *      - ESP_OK                Set level success
 */
esp_err_t set_icm42670_log_level(esp_log_level_t log_level);