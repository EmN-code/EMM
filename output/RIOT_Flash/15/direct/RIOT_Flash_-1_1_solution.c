#include <stdio.h>
#include "xtimer.h"
#include "nvs.h"
#include "nvs_flash.h"

#define INTERVAL (1U * US_PER_SEC)

static nvs_handle_t my_handle;
static const char *storage_namespace = "storage";

static void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS storage with the specified namespace
    err = nvs_open(storage_namespace, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);
}

static void toggle_device_state(void) {
    uint8_t device_state;
    size_t required_size = sizeof(device_state);
    esp_err_t err = nvs_get_u8(my_handle, "device_state", &device_state);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        // Value not initialized yet, let's set it to 0
        device_state = 0;
    } else if (err != ESP_OK) {
        printf("Error (%s) reading!
", esp_err_to_name(err));
        return;
    }

    // Toggle state
    device_state = !device_state;

    // Write updated value back into NVS
    err = nvs_set_u8(my_handle, "device_state", device_state);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
    }

    // Commit changes to NVS
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing!
", esp_err_to_name(err));
    }

    printf("Device state is now: %d\n", device_state);
}

int main(void) {
    init_nvs();

    while (1) {
        toggle_device_state();
        xtimer_sleep(INTERVAL);
    }

    // Close NVS handle after use
    nvs_close(my_handle);
    return 0;
}