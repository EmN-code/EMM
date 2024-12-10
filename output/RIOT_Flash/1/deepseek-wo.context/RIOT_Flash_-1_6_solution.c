
#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "xtimer.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    // Initialize NVS
    nvs_flash_init();

    // Open NVS
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    // Main loop
    while (1) {
        // Simulate device state (1 or 0)
        int device_state = (xtimer_now_usec() % 2 == 0) ? 1 : 0;

        // Store device state in NVS
        err = nvs_set_i32(my_handle, NVS_KEY, device_state);
        if (err != ESP_OK) {
            printf("Error (%d) writing to NVS!\n", err);
        } else {
            printf("Device state %d written to NVS\n", device_state);
        }

        // Commit changes to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing to NVS!\n", err);
        }

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
