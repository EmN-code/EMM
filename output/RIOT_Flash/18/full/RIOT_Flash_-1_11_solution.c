#include "xtimer.h"
#include "string.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"
#define STORAGE_KEY "device_state"

// Function to initialize NVS
esp_err_t initialize_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    return err;
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int32_t device_state = 0;

    // Initialize NVS
    err = initialize_nvs();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return err;
    }

    // Open
    while (true) {
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            xtimer_sleep(1);
            continue;
        }

        // Read the state
        err = nvs_get_i32(my_handle, STORAGE_KEY, &device_state);
        switch (err) {
            case ESP_OK:
                printf("Device state = %" PRIi32 "\n", device_state);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        // Change the state for demonstration
        device_state = !device_state;

        // Write
        err = nvs_set_i32(my_handle, STORAGE_KEY, device_state);
        if (err != ESP_OK) {
            printf("Error (%s) writing!\n", esp_err_to_name(err));
        }

        // Commit
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing!\n", esp_err_to_name(err));
        }

        // Close
        nvs_close(my_handle);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    return 0;
}