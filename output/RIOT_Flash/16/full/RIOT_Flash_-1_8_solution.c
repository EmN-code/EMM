#include <stdio.h>
#include <string.h>
#include <esp_err.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

// Initialize NVS
static esp_err_t init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    return err;
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    const char *nvs_namespace = "storage";
    const char *state_key = "device_state";
    uint8_t state = 0;

    // Initialize NVS
    while (1) {
        err = init_nvs();
        if (err == ESP_OK) {
            printf("NVS initialized successfully.\n");
            break;
        } else {
            printf("Error (%s) initializing NVS, retrying...\n", esp_err_to_name(err));
            xtimer_sleep(1);
        }
    }

    // Open NVS handle
    while (1) {
        err = nvs_open(nvs_namespace, NVS_READWRITE, &my_handle);
        if (err == ESP_OK) {
            printf("NVS handle opened successfully.\n");
            break;
        } else {
            printf("Error (%s) opening NVS handle, retrying...\n", esp_err_to_name(err));
            xtimer_sleep(1);
        }
    }

    // Loop to continuously update the device state
    while (1) {
        state = (state == 0) ? 1 : 0;
        err = nvs_set_u8(my_handle, state_key, state);
        printf((err != ESP_OK) ? "Failed to set value in NVS!\n" : "Value set successfully.
");

        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed to commit to NVS!\n" : "NVS commit successful.
");

        xtimer_sleep(1);
    }

    // Close nvs handle before exit
    nvs_close(my_handle);

    return 0;
}