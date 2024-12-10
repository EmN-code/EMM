#include <stdio.h>  // Standard I/O operations
#include <string.h> // For string operations
#include "nvs_flash.h" // NVS Flash for storage operations
#include "nvs.h" // NVS for handling the non-volatile storage
#include "esp_system.h" // ESP32 system and boot
#include "xtimer.h" // Timer utilities for RIOT-OS

void save_state_to_nvs(const char *state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open NVS in read/write mode
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write the state to NVS
    err = nvs_set_str(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Failed to write to NVS!\n");
    }
    
    // Commit the written value to NVS
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit changes to NVS!\n");
    }

    // Close the NVS handle
    nvs_close(my_handle);
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    printf("NVS initialized!\n");

    while (1) {
        // Example state update ("1" or "0")
        const char *state = "1";
        save_state_to_nvs(state);
        
        // Simulate a periodic update every second
        xtimer_sleep(1);

        // Toggle the state
        state = (strcmp(state, "1") == 0) ? "0" : "1";
    }

    return 0;
}