#ifndef SENSOR_TEMP_DS18B20_H
#define SENSOR_TEMP_DS18B20_H

/**
 * @file sensor_temp_ds18b20.h
 * @brief Header file for the DS18B20 temperature sensor component.
 */

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the DS18B20 temperature sensor.
 * 
 * This function initializes the DS18B20 temperature sensor by specifying the GPIO pin to which it is connected.
 * 
 * @param pin The GPIO pin number to which the DS18B20 sensor is connected.
 * @return esp_err_t Returns ESP_OK if the initialization is successful, otherwise an error code.
 */
esp_err_t sensor_temp_ds18b20_init(int pin);

/**
 * @brief Read the temperature from the DS18B20 sensor.
 * 
 * This function reads the temperature from the DS18B20 sensor and stores it in the provided temperature variable.
 * 
 * @param temperature Pointer to a float variable where the temperature will be stored.
 * @return esp_err_t Returns ESP_OK if the temperature reading is successful, otherwise an error code.
 */
esp_err_t sensor_temp_ds18b20_read_temperature(float* temperature);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_TEMP_DS18B20_H */
