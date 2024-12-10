
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "nvs.h"

#define NVS_NAMESPACE "storage"
#define TEMPERATURE_KEY "temperature"

int main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t temp = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash Init failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS Open failed: %d\n", err);
        return -1;
    }

    while (1) {
        // Simulate temperature change
        temp = (temp == 0) ? 1 : 0;

        // Write temperature to NVS
        err = nvs_set_i32(my_handle, TEMPERATURE_KEY, temp);
        if (err != ESP_OK) {
            printf("NVS Set failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Commit the write operation
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS Commit failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Print the current temperature
        printf("Temperature updated to: %ld\n", (long)temp);

        // Wait for 1 second
        sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
