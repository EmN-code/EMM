#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <nvs_flash.h>
#include <nvs.h>

void init_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated
        // and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        // Retry nvs_flash_init
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void update_device_state() {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    } else {
        // Logic to toggle state between 1 and 0
        int32_t state = 0;
        err = nvs_get_i32(my_handle, "state", &state);
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            state = 0;  // Default state
        }
        state = !state;
        err = nvs_set_i32(my_handle, "state", state);

        printf((err != ESP_OK) ? "Failed to update NVS!\n" : "Device state updated to %d\n", state);

        // Commit written value.
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed to commit NVS!\n" : "NVS committed successfully!\n");

        // Close
        nvs_close(my_handle);
    }
}

int main() {
    // Initialize NVS
    init_nvs();
    
    while (true) {
        update_device_state();
        sleep(1); // Sleep for 1 second to simulate task delay
    }
    return 0;
}