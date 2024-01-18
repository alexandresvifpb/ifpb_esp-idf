/**
 * @file main.c
 * @brief This file contains the main application code for reading temperature using DS18B20 sensor.
 * @author Alexandre Sales Vasconcelos
 * @email alexandre.vasconcelos@ifpb.edu.br
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "sdkconfig.h"
#include "esp_log.h"

#include "sensor_temp_ds18b20.h"

// foi preciso colocar esse ifndef para que o compilador nao reclamasse
#ifndef CONFIG_FREERTOS_HZ
#define CONFIG_FREERTOS_HZ 1000
#endif

#define TAG "main"
#define CONFIG_SENSOR_TEMP_DS18B20_PIN 4

static float temperature = 0.0f;

/**
 * @brief Task to read temperature from DS18B20 sensor periodically.
 * 
 * This task initializes the DS18B20 sensor and reads the temperature value
 * at regular intervals. The temperature value is then logged using ESP_LOGI.
 * The task delays for 1 second between each temperature reading.
 * 
 * @param pvParameters Pointer to task parameters (not used in this task).
 */
void temperature_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Initializing DS18B20 sensor");
    sensor_temp_ds18b20_init(CONFIG_SENSOR_TEMP_DS18B20_PIN);
    while (1) {
        sensor_temp_ds18b20_read_temperature(&temperature);
        ESP_LOGI(TAG, "Temperature: %.2f°C", temperature);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief The entry point of the application.
 * 
 * This function is called by the system when the application starts.
 * It creates a task to read temperature from DS18B20 sensor.
 */
void app_main(void)
{
    xTaskCreate(temperature_task, "temperature_task", 2048, NULL, 5, NULL);
}