#include "nvs_flash.h"
#include "nvs.h"
#include "periph/gpio.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"
#define PIN 4 // Assuming GPIO 4 is used, adjust as necessary

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return err;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    // Retrieve stored device state (0 or 1)
    uint8_t device_state = 0;
    nvs_get_u8(my_handle, "device_state", &device_state);

    // Toggle device state every second
    while (1) {
        device_state = !device_state; // Toggle between 0 and 1
        printf("Storing device state: %d\n", device_state);

        // Store the new device state
        err = nvs_set_u8(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Failed to set device state (%s)!\n", esp_err_to_name(err));
        }

        // Commit to save the new value
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit (%s)!\n", esp_err_to_name(err));
        }

        xtimer_sleep(1); // Sleep for 1 second
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}