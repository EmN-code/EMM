#include <stdio.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"
#define KEY "device_state"

void save_device_state(nvs_handle_t handle, int state) {
    esp_err_t err;
    printf("Saving device state: %d\n", state);
    err = nvs_set_i32(handle, KEY, state);
    if (err != ESP_OK) {
        printf("Error (%d) saving device state!\n", err);
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%d) committing updates!\n", err);
    }
}

int load_device_state(nvs_handle_t handle) {
    int32_t state = 0;  // Default state
    esp_err_t err = nvs_get_i32(handle, KEY, &state);
    switch (err) {
        case ESP_OK:
            printf("Loaded device state: %d\n", state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("Device state not found\n");
            break;
        default :
            printf("Error (%d) reading device state\n", err);
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    nvs_handle_t handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    int device_state = load_device_state(handle);

    while (1) {
        // Toggles the device state
        device_state = !device_state;
        save_device_state(handle, device_state);
        xtimer_sleep(1);  // Sleep for 1 second
    }

    nvs_close(handle);
    return 0;
}