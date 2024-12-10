#include "xtimer.h"
#include "nvs_flash.h"
#include "nvs.h"

#define DELAY 1  // delay time in seconds

void store_device_state(uint8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Open
    while (1) {
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle! Retrying...\n", esp_err_to_name(err));
            xtimer_sleep(DELAY);
            continue;
        } else {
            printf("NVS handle opened successfully.\n");
            break;
        }
    }
    
    // Write
    err = nvs_set_u8(my_handle, "device_state", state);
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
}

int main(void) {
    esp_err_t err = nvs_flash_init();  // Initialize NVS
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    uint8_t state = 0;  // Device state to be stored

    while (1) {
        state = !state;  // Toggle state between 1 and 0
        store_device_state(state);
        printf("Stored state: %d\n", state);
        xtimer_sleep(DELAY);
    }

    return 0;
}