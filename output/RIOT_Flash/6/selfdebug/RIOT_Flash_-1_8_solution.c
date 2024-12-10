#include <zephyr.h>  // for using k_sleep
#include <stdio.h>
#include "nvs.h"   // For NVS functions

/**
 * @brief   Main function entry point
 */
int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t temperature = 25;

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    while (1) {
        // Writing temperature to NVS
        err = nvs_set_i32(my_handle, "temperature", temperature);
        if (err != ESP_OK) {
            printf("Failed to set temperature\n");
        }

        // Commit written value to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit changes\n");
        } else {
            printf("Temperature %ld written successfully\n", (long) temperature);
        }

        // Change the state of the device
        temperature = (temperature == 1) ? 0 : 1;

        // Sleep for 1 second
        k_sleep(K_SECONDS(1));
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}