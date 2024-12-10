#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <assert.h>

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    // Write the state to NVS
    err = nvs_set_i32(my_handle, "device_state", state);
    ESP_ERROR_CHECK(err);

    // Commit written value.
    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);

    // Close the handle
    nvs_close(my_handle);
}

int read_device_state() {
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 0; // Default state if not found

    // Open
    err = nvs_open("storage", NVS_READONLY, &my_handle);
    if (err != ESP_OK) return state;

    // Read
    err = nvs_get_i32(my_handle, "device_state", &state);
    if (err != ESP_OK) state = 0;

    // Close
    nvs_close(my_handle);

    return state;
}

int main(void) {
    printf("Initializing NVS Example...\n");

    while(1) {
        int current_state = read_device_state();
        printf("Current Device State: %d\n", current_state);

        // Toggle state
        int new_state = current_state == 0 ? 1 : 0;
        store_device_state(new_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }
    return 0;
}