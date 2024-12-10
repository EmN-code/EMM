#include "xtimer.h"
#include "nvs_flash.h"
#include "stdint.h"
#include "stdlib.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"
#define STATE_UPDATE_INTERVAL (1 * US_PER_SEC)

void store_device_state(uint8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to set device state in NVS (%d)!\n", err);
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit updates to NVS (%d)!\n", err);
    }

    nvs_close(my_handle);
}

uint8_t get_device_state() {
    nvs_handle_t my_handle;
    uint8_t state = 0;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &my_handle);
    if (err == ESP_OK) {
        err = nvs_get_u8(my_handle, DEVICE_STATE_KEY, &state);
        if (err != ESP_OK) {
            printf("Failed to read device state from NVS (%d)!\n", err);
        }
        nvs_close(my_handle);
    } else {
        printf("Error (%d) opening NVS handle for reading!\n", err);
    }
    return state;
}

int main(void) {
    uint8_t device_state;

    if (nvs_flash_init() != ESP_OK) {
        printf("NVS flash initialization failed. Attempting erase...\n");
        if (nvs_flash_erase() == ESP_OK && nvs_flash_init() == ESP_OK) {
            printf("NVS flash erased and initialized successfully.\n");
        } else {
            printf("Failed to initialize NVS flash even after erase.\n");
            return -1;
        }
    }

    device_state = get_device_state();
    printf("Current device state: %d\n", device_state);

    while (1) {
        device_state = (device_state == 0) ? 1 : 0;
        store_device_state(device_state);
        printf("Stored new device state: %d\n", device_state);

        xtimer_usleep(STATE_UPDATE_INTERVAL);
    }

    return 0;
}
