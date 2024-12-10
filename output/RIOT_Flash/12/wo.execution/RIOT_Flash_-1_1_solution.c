#include <thread_flags.h>
#include <stddef.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_system.h>

int main() {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        return err;
    }

    // Open NVS handle
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        return err;
    }

    int16_t temperature = 0;
    // Placeholder function to read temperature, here it is simulated
    int16_t read_temperature() {
        // Simulate reading temperature which changes every second
        return (rand() % 100); // Random temperature between 0 and 99
    }

    while (1) {
        // Read current temperature
        temperature = read_temperature();

        // Update the state in NVS
        err = nvs_set_i16(my_handle, "temperature", temperature);
        if (err != ESP_OK) {
            nvs_close(my_handle);
            return err;
        }

        // Commit the updated value to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            nvs_close(my_handle);
            return err;
        }

        // Print the stored device state for demonstration purposes
        printf("Stored temperature in NVS: %d\n", temperature);

        // Sleep for 1 second
        thread_sleep();
    }

    // Close NVS handle
    nvs_close(my_handle);
    return 0;
}