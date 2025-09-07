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
  i2c_port_t i2c_master_port;
  icm42670_acce_pwr_t acce_pwr;
  icm42670_gyro_pwr_t gyro_pwr;
  icm42670_handle_t *icm_sensor;
  icm42670_cfg_t *sensor_cfg;
} icm42670_esp_config_t;

esp_err_t init_icm42670(icm42670_esp_config_t *esp_config);
float calculate_tilt_angle(float x, float y, float z);

esp_err_t set_icm42670_log_level(esp_log_level_t log_level);