#include <stdio.h>  // Standard input/output definitions
#include <nvs_flash.h>  // NVS flash for using non-volatile storage
#include <nvs.h>  // Header for Non-Volatile Storage library
#include <string.h>  // String operations
#include <stdint.h>  // Standard types
#include <stdbool.h>  // Boolean definitions

// Function to initialize NVS
void init_nvs(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

// Function to read device state from NVS
int32_t read_device_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t device_state = 0;  // Default state
    
    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return device_state;

    // Read state
    err = nvs_get_i32(my_handle, "device_state", &device_state);
    switch (err) {
        case ESP_OK:
            printf("Device state = %ld\n", device_state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet.\n");
            break;
        default :
            printf("Error (%s) reading!
", esp_err_to_name(err));
    }

    nvs_close(my_handle);
    return device_state;
}

// Function to write device state to NVS
void write_device_state(int32_t device_state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return;

    // Write
    err = nvs_set_i32(my_handle, "device_state", device_state);
    if (err != ESP_OK) return;

    // Commit written value.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) return;

    // Close
    nvs_close(my_handle);
}

void update_device_state() {
    int32_t state = read_device_state();
    state = !state;  // Toggle state between 0 and 1
    write_device_state(state);
}

int main(void) {
    init_nvs();

    while (true) {
        update_device_state();
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay for 1 second
    }

    return 0;
}