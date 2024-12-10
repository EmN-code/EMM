#include "nvs_flash.h"
#include "nvs.h"
#include "stdio.h"  // For printf
#include "xtimer.h" // For time management

#define STORAGE_NAMESPACE "storage"
#define TEMP_KEY "temperature"

void app_main(void) {
    int16_t temperature;
    nvs_handle_t nvs_handle;

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return;
    }

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Attempt to read the temperature from NVS
    err = nvs_get_i16(nvs_handle, TEMP_KEY, &temperature);
    switch (err) {
        case ESP_OK:
            printf("Temperature = %d\n", temperature);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("Temperature not found, initializing to 0\n");
            temperature = 0;
            break;
        default :
            printf("Error (%s) reading temperature!\n", esp_err_to_name(err));
            return;
    }

    // Example loop to simulate temperature change and write to NVS
    for (int i = 0; i < 10; i++) {  // run the loop 10 times
        // Simulate temperature change
        temperature = (temperature + 1) % 100;

        // Write the updated temperature back to NVS
        err = nvs_set_i16(nvs_handle, TEMP_KEY, temperature);
        if (err != ESP_OK) {
            printf("Error (%s) saving temperature!\n", esp_err_to_name(err));
        }

        // Commit written value to NVS
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing temperature!\n", esp_err_to_name(err));
        }

        printf("Updated Temperature = %d\n", temperature);

        // Simulate a delay (e.g., 1 second)
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(nvs_handle);
}

int main(void) {
    app_main();
    return 0;
}