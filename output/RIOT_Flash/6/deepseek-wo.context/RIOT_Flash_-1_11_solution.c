
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    int state = 1;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS Open failed: %d\n", err);
        return -1;
    }

    while (1) {
        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Store state in NVS
        err = nvs_set_i32(my_handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("NVS Set failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Commit changes
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS Commit failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Print current state
        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
