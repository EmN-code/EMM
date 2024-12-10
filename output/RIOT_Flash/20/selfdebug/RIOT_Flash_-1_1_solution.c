#include "nvs_flash.h"
#include "nvs.h"
#include "thread.h"
#include <stdio.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static char stack[THREAD_STACKSIZE_DEFAULT];

void nvs_task(void) {
    nvs_handle_t nvs_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    while (1) {
        int16_t temperature;
        // Simulate temperature value; in practice, you'd get this from a real sensor
        temperature = 25; // Replace with actual reading

        // Open the NVS namespace
        err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%d) opening NVS handle!\n", err);
        } else {
            // Write temperature to NVS
            err = nvs_set_i16(nvs_handle, NVS_KEY, temperature);
            printf((err != ESP_OK) ? "Failed!\n" : "Temperature written\n");

            // Commit written value to NVS
            err = nvs_commit(nvs_handle);
            printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

            // Close NVS
            nvs_close(nvs_handle);
        }

        // Wait 1 second (1000 ms)
        xtimer_sleep(1);
    }
}

int main(void) {
    // Start a new thread for the NVS task
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, nvs_task, NULL, "nvs_task");

    // The main function can continue doing other things or just loop forever
    while (1) {
        // Stay idle
        thread_sleep();
    }
    return 0;
}
