#include <xtimer.h>
#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

static void store_device_state(const char *key, int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    // Write
    err = nvs_set_i32(my_handle, key, state);
    if (err != ESP_OK) {
        printf("Error (%d) writing to NVS!\n", err);
        nvs_close(my_handle);
        return;
    }

    // Commit written value.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) committing changes!\n", err);
    }

    // Close
    nvs_close(my_handle);
}

static int read_device_state(const char *key) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 0; // Default value

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return state;
    }

    // Read
    err = nvs_get_i32(my_handle, key, &state);
    switch (err) {
        case ESP_OK:
            printf("Read value = %d\n", state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%d) reading!
", err);
    }

    // Close
    nvs_close(my_handle);
    return state;
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return err;
    }

    const char *key = "device_state";
    int state = 0;

    while (1) {
        // Simulate temperature update and storing device state to NVS every second
        state = (state == 0) ? 1 : 0; // Toggle state
        store_device_state(key, state);
        xtimer_sleep(1);
    }

    return 0;
}
