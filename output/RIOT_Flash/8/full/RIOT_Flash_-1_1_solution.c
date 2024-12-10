#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    while (1) {
        // Open
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            xtimer_sleep(1);
            continue;
        } else {
            // Read
            int32_t state = 0; // Default to 0 if not previously set
            err = nvs_get_i32(my_handle, "device_state", &state);
            switch (err) {
                case ESP_OK:
                    printf("Current state = %d\n", state);
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    printf("The value is not initialized yet!\n");
                    break;
                default :
                    printf("Error (%s) reading!
", esp_err_to_name(err));
            }

            // Update the state
            state = (state == 0) ? 1 : 0;
            printf("Writing new state: %d\n", state);
            err = nvs_set_i32(my_handle, "device_state", state);
            if (err != ESP_OK) {
                printf("Failed to write state (%s)!\n", esp_err_to_name(err));
            }

            // Commit written value
            err = nvs_commit(my_handle);
            nvs_close(my_handle);
            if (err != ESP_OK) {
                printf("Failed to commit state (%s)!\n", esp_err_to_name(err));
            }
        }

        // Simulation of delay as per configuration
        printf("Sleeping for 1 second before next write...\n");
        xtimer_sleep(1);
    }

    return 0;
}