
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 1; // Initial state

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        while (1) {
            // Toggle state
            state = (state == 1) ? 0 : 1;

            // Write state to NVS
            err = nvs_set_i32(my_handle, NVS_KEY, state);
            if (err != ESP_OK) {
                printf("Error (%d) writing to NVS!\n", err);
            } else {
                printf("State written to NVS: %d\n", state);
            }

            // Commit written value. After setting any values, nvs_commit() must be called to ensure changes are written to flash storage.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%d) committing to NVS!\n", err);
            }

            // Wait for 1 second
            xtimer_sleep(1);
        }

        // Close NVS
        nvs_close(my_handle);
    }

    return 0;
}
