#include "nvs_flash.h"
#include "nvs.h"
#include "stdio.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"

void store_device_state(nvs_handle_t handle) {
    esp_err_t err;
    int32_t state = 0; // Initially, setting the state to 0

    // Read current state
    err = nvs_get_i32(handle, "device_state", &state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("State not initialized yet!");
    }

    // Toggle state
    state = !state;

    // Write back the new state
    err = nvs_set_i32(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Failed to write state to NVS!");
    }

    // Commit written value.
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Failed to commit state to NVS!");
    }
    printf("Stored device state: %d\n", state);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return err;
    }

    while (1) {
        store_device_state(my_handle);
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}