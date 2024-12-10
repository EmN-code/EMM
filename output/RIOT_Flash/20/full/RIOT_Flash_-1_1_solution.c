#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"

void init_nvs(nvs_handle_t *handle) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing flash: %s\n", esp_err_to_name(err));
        return;
    }
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return;
    }
}

void store_device_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting NVS key-value: %s\n", esp_err_to_name(err));
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error committing updates in NVS: %s\n", esp_err_to_name(err));
    } else {
        printf("Device state %d stored to NVS\n", state);
    }
}

void read_device_state(nvs_handle_t handle, uint8_t *state) {
    esp_err_t err = nvs_get_u8(handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error reading NVS key-value: %s\n", esp_err_to_name(err));
    } else {
        printf("Device state read from NVS is %d\n", *state);
    }
}

int main(void) {
    nvs_handle_t nvs_handle;
    init_nvs(&nvs_handle);

    uint8_t device_state = 0;

    while (1) {
        store_device_state(nvs_handle, device_state);
        read_device_state(nvs_handle, &device_state);

        // Toggle device state between 0 and 1
        device_state = (device_state == 0) ? 1 : 0;

        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}