
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t handle;
    esp_err_t err;
    int32_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        // Toggle state
        state = (state == 0) ? 1 : 0;

        // Write state to NVS
        err = nvs_set_i32(handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("NVS set failed: %d\n", err);
            nvs_close(handle);
            return -1;
        }

        // Commit changes
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(handle);
            return -1;
        }

        // Read state from NVS
        err = nvs_get_i32(handle, NVS_KEY, &state);
        if (err != ESP_OK) {
            printf("NVS get failed: %d\n", err);
            nvs_close(handle);
            return -1;
        }

        // Print state
        printf("Device state: %ld\n", (long)state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(handle);

    return 0;
}
