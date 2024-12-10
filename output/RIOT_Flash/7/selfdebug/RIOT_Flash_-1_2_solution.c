#include "task.h"
#include "nvs_flash.h"
#include <stdio.h>

#define STORAGE_KEY "device_state"
#define DELAY_MS 1000 // Delay in milliseconds

void update_device_state(int state) {
    nvs_handle_t handle;
    esp_err_t err;

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write the new state to NVS
    err = nvs_set_i32(handle, STORAGE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to write to NVS!\n");
    }

    // Commit the updates to NVS
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Failed to commit NVS!\n");
    }

    // Close the handle
    nvs_close(handle);
}

void device_state_task(void *pvParameter) {
    int state = 0;
    while (1) {
        update_device_state(state);
        // Toggle the device state between 1 and 0
        state = !state;
        // Delay for DELAY_MS milliseconds
        vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
    }
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS (%s)!\n", esp_err_to_name(err));
        return 1;
    }

    // Create a FreeRTOS task to update the device state
    xTaskCreate(&device_state_task, "device_state_task", 2048, NULL, 5, NULL);

    // Start the scheduler
    vTaskStartScheduler();
    
    return 0;
}