#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* --- Pin Definitions --- */
#define LED_PIN PICO_DEFAULT_LED_PIN

/* --- Task Priorities --- */
#define SENSOR_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define LED_TASK_PRIORITY    (tskIDLE_PRIORITY + 1)

/* --- Global Variables --- */
// A semaphore to signal from the sensor task to the LED task
SemaphoreHandle_t xTemperatureSemaphore;

// Temperature threshold for triggering the LED
const float TEMPERATURE_THRESHOLD = 25.0f;

/*
 * Converts the raw ADC value from the internal temperature sensor
 * to a temperature in Celsius.
 */
float read_onboard_temperature() {
    // Select ADC input 4 (the temperature sensor)
    adc_select_input(4);

    // Perform the conversion
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = adc_read();
    float voltage = result * conversion_factor;
    
    // Formula from the datasheet to convert voltage to temperature
    float temp_celsius = 27.0f - (voltage - 0.706f) / 0.001721f;
    return temp_celsius;
}

/* --- Task Implementations --- */

/**
 * @brief Task to read the internal temperature sensor periodically.
 *
 * This task reads the temperature every 2 seconds. If the temperature
 * exceeds TEMPERATURE_THRESHOLD, it gives a semaphore to signal the LED task.
 */
void vSensorTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(2000); // Run every 2000 ms

    printf("Sensor Task started.\n");

    for (;;) {
        // Wait for the next cycle.
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        float temperature = read_onboard_temperature();
        printf("Sensor Task: Current temperature is %.2f C\n", temperature);

        // If temperature is above the threshold, signal the LED task
        if (temperature > TEMPERATURE_THRESHOLD) {
            printf("Sensor Task: Temperature threshold exceeded! Signaling LED task.\n");
            xSemaphoreGive(xTemperatureSemaphore);
        }
    }
}

/**
 * @brief Task to control the onboard LED.
 *
 * This task waits indefinitely for a signal (semaphore) from the sensor task.
 * Upon receiving the signal, it toggles the LED.
 */
void vLedTask(void *pvParameters) {
    printf("LED Task started. Waiting for temperature alert...\n");

    for (;;) {
        // Block indefinitely until the semaphore is given by the sensor task
        if (xSemaphoreTake(xTemperatureSemaphore, portMAX_DELAY) == pdTRUE) {
            // We received the signal!
            printf("LED Task: Signal received. Toggling LED.\n");
            gpio_put(LED_PIN, !gpio_get(LED_PIN)); // Toggle the LED state
        }
    }
}


int main() {
    // Initialize standard libraries
    stdio_init_all();
    
    // Initialize hardware
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    adc_init();
    adc_set_temp_sensor_enabled(true);

    printf("--- FreeRTOS RTOS Task Scheduler Demo ---\n");

    // Create the binary semaphore.
    // It is created in a "taken" state, so the LED task will block immediately.
    xTemperatureSemaphore = xSemaphoreCreateBinary();
    if (xTemperatureSemaphore == NULL) {
        printf("Error: Could not create semaphore.\n");
        while(1); // Halt
    }

    // Create the tasks
    xTaskCreate(vSensorTask, "SensorTask", 256, NULL, SENSOR_TASK_PRIORITY, NULL);
    xTaskCreate(vLedTask, "LedTask", 256, NULL, LED_TASK_PRIORITY, NULL);
    
    printf("Tasks created. Starting scheduler.\n");

    // Start the scheduler. This function will never return.
    vTaskStartScheduler();

    // The following line should never be reached.
    for (;;);
    
    return 0; // Should not be reached
}