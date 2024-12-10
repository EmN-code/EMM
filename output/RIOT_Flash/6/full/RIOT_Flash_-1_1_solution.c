#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include "xtimer.h"

#define NAMESPACE "storage"
#define KEY "device_state"
#define INTERVAL 1   // Time in seconds

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

int read_device_state(nvs_handle_t handle) {
    int32_t device_state = 0;
    esp_err_t err = nvs_get_i32(handle, KEY, &device_state);
    if (err != ESP_OK) {
        printf("Failed to read device state from NVS!
");
    }
    return device_state;
}

void write_device_state(nvs_handle_t handle, int32_t state) {
    esp_err_t err = nvs_set_i32(handle, KEY, state);
    if (err != ESP_OK) {
        printf("Failed to write device state to NVS!
");
    }
    nvs_commit(handle);
}

void update_device_state(nvs_handle_t handle) {
    int32_t current_state = read_device_state(handle);
    int32_t new_state = current_state == 1 ? 0 : 1; // Toggle the state
    printf("Updating device state to: %d\n", new_state);
    write_device_state(handle, new_state);
}

int main(void) {
    nvs_handle_t nvs_handle;

    /* Initialize NVS */
    init_nvs();

    /* Open NVS handle */
    esp_err_t err = nvs_open(NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return -1;
    }

    /* Run update loop */
    while (1) {
        update_device_state(nvs_handle);
        xtimer_sleep(INTERVAL);
    }

    /* Close NVS handle */
    nvs_close(nvs_handle);

    return 0;
}