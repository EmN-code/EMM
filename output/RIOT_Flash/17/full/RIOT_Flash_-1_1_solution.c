#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define NAMESPACE "storage"
#define KEY "device_state"
#define SLEEP_INTERVAL (1U)

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int32_t state = 1;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open(NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        // Write the state to NVS
        err = nvs_set_i32(my_handle, KEY, state);
        if (err != ESP_OK) {
            printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
        } else {
            printf("Device state saved: %d\n", state);

            // Commit written value.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing value!\n", esp_err_to_name(err));
            }

            // Toggle state value
            state = (state == 1) ? 0 : 1;
        }

        // Wait for next update
        xtimer_sleep(SLEEP_INTERVAL);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}