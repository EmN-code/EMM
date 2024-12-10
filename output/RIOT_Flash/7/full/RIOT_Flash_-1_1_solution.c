#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_err.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"
#define TAG "nvs"
#define SLEEP_INTERVAL 1000000 // 1 second in microseconds

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS: %s\n", esp_err_to_name(err));
        return -1;
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Simulated temperature value
    uint8_t temperature = 0;

    while (1) {
        // Toggle temperature value for demonstration
        temperature = temperature ? 0 : 1;

        // Write value to NVS
        err = nvs_set_u8(my_handle, "temperature", temperature);
        if (err != ESP_OK) {
            printf("Error setting NVS value: %s\n", esp_err_to_name(err));
            xtimer_sleep(1);
            continue;
        }

        // Commit written value
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing NVS value: %s\n", esp_err_to_name(err));
        } else {
            printf("Temperature saved as: %d\n", temperature);
        }

        // Sleep for a while
        xtimer_usleep(SLEEP_INTERVAL);
    }

    // Close NVS handle on clean-up
    nvs_close(my_handle);

    return 0;
}