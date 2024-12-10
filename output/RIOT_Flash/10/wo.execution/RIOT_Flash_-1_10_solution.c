#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"
#define INTERVAL 1000000 // 1 second in microseconds

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int32_t temp = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return err;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        // Write temperature to NVS
        err = nvs_set_i32(my_handle, "temperature", temp);
        if (err != ESP_OK) {
            printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
        } else {
            nvs_commit(my_handle);
            printf("Temperature (%d) committed to NVS\n", temp);
        }

        // Simulating temperature change
        temp = (temp == 0) ? 1 : 0;

        // Sleep for 1 second
        xtimer_usleep(INTERVAL);
    }

    // Close NVS handle
    nvs_close(my_handle);
    return ESP_OK;
}