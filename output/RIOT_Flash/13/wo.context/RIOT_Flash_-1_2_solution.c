#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"

void store_state_to_nvs(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) setting NVS data!\n", esp_err_to_name(err));
    }

    // Commit written value.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing NVS data!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(my_handle);
}

void toggle_device_state(void) {
    static int state = 0;
    state = !state; // Toggle between 0 and 1
    store_state_to_nvs(state);
    printf("Device state stored: %d\n", state);
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    while (1) {
        toggle_device_state();
        xtimer_sleep(1);
    }
    return 0;
}