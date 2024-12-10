#include <stdio.h>
#include <stdlib.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"

void initialize_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated, erase it
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
        ESP_ERROR_CHECK(err);
    }
    ESP_ERROR_CHECK(err);
}

int main(void) {
    initialize_nvs();

    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 0;
    
    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        // Read the state, defaulting to 0 if not found
        err = nvs_get_i32(my_handle, "device_state", &state);
        switch (err) {
            case ESP_OK:
                printf("Read state = %d\n", state);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        state = !state; // Toggle state
        
        // Write updated state back
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error (%s) writing the state!
", esp_err_to_name(err));
        }

        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing the state!
", esp_err_to_name(err));
        }

        // Close
        nvs_close(my_handle);
    }

    // Delay of 1 second
    xtimer_sleep(1);

    return 0;
}
