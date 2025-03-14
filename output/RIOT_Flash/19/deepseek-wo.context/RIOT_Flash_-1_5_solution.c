
#include <stdio.h>
#include <xtimer.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

int main(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    nvs_handle_t my_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("NVS handle opened successfully!\n");

        // Loop to update device state every second
        while (1) {
            // Simulate device state (1 or 0)
            int32_t device_state = (xtimer_now().ticks32 % 2);

            // Write device state to NVS
            err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, device_state);
            if (err != ESP_OK) {
                printf("Error (%s) writing device state to NVS!\n", esp_err_to_name(err));
            } else {
                printf("Device state %d written to NVS\n", device_state);
            }

            // Commit written value.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
            }

            // Wait for 1 second
            xtimer_sleep(1);
        }

        // Close NVS
        nvs_close(my_handle);
    }

    return 0;
}
