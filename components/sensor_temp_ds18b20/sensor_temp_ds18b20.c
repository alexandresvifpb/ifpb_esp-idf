/**
 * @file sensor_temp_ds18b20.c
 * @brief Implementation of the DS18B20 temperature sensor module.
 * @author Alexandre Sales Vasconcelos
 * @author Alexandre Sales Vasconcelos
 * @email alexandre.vasconcelos@ifpb.edu.br
 */

#include <stdio.h>
#include "sensor_temp_ds18b20.h"
#include "esp_log.h"
#include "esp_err.h"

#include "ds18b20.h"
#include "onewire_bus.h"

#define TAG "sensor_temp_ds18b20"

static ds18b20_device_handle_t sensor = NULL;

/**
 * @brief Initialize the DS18B20 temperature sensor.
 *
 * This function initializes the DS18B20 temperature sensor by installing the 1-wire bus,
 * creating a device iterator, and searching for the DS18B20 device.
 *
 * @param pin The GPIO pin number to which the DS18B20 sensor is connected.
 * @return esp_err_t Returns ESP_OK if the initialization is successful, otherwise returns an error code.
 */
esp_err_t sensor_temp_ds18b20_init(int pin)
{
    // install 1-wire bus
    onewire_bus_handle_t bus = NULL;
    onewire_bus_config_t bus_config = {
        .bus_gpio_num = pin,
    };
    onewire_bus_rmt_config_t rmt_config = {
        .max_rx_bytes = 10, // 1byte ROM command + 8byte ROM number + 1byte device command
    };
    ESP_ERROR_CHECK(onewire_new_bus_rmt(&bus_config, &rmt_config, &bus));
    onewire_device_iter_handle_t iter = NULL;
    onewire_device_t next_onewire_device;
    esp_err_t search_result = ESP_OK;
    // create 1-wire device iterator, which is used for device search
    ESP_ERROR_CHECK(onewire_new_device_iter(bus, &iter));
    ESP_LOGI(TAG, "Device iterator created, start searching...");
    search_result = onewire_device_iter_get_next(iter, &next_onewire_device);
    if (search_result == ESP_OK)
    { // found a new device, let's check if we can upgrade it to a DS18B20
        ds18b20_config_t ds_cfg = {};
        // check if the device is a DS18B20, if so, return the ds18b20 handle
        if (ds18b20_new_device(&next_onewire_device, &ds_cfg, &sensor) == ESP_OK)
        {
            ESP_LOGI(TAG, "Found a DS18B20, address: %016llX", next_onewire_device.address);
            ESP_ERROR_CHECK(onewire_del_device_iter(iter));
            ESP_LOGI(TAG, "Iterretor deleted");
        }
        else
        {
            ESP_LOGI(TAG, "Found an unknown device, address: %016llX", next_onewire_device.address);
            return ESP_FAIL;
        }
    } // else, no device found, just return
    else
    {
        ESP_LOGE(TAG, "No devices found, please check connection");
        return ESP_FAIL;
    }
    return ESP_OK;
}

/**
 * @brief Read the temperature from the DS18B20 sensor.
 *
 * This function triggers a temperature conversion on the DS18B20 sensor and reads the temperature value.
 *
 * @param temperature Pointer to a float variable to store the temperature value.
 * @return esp_err_t Returns ESP_OK if the temperature reading is successful, otherwise returns an error code.
 */
esp_err_t sensor_temp_ds18b20_read_temperature(float *temperature)
{
    ESP_ERROR_CHECK(ds18b20_trigger_temperature_conversion(sensor));
    ESP_ERROR_CHECK(ds18b20_get_temperature(sensor, temperature));
    return ESP_OK;
}