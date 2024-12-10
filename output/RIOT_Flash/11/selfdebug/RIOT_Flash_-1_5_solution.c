#include <stdio.h>
#include <inttypes.h>
#include <esp_err.h>
#include <esp_partition.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define TEMPERATURE_KEY "temperature"

static nvs_handle_t my_handle;

int main(void) {
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

    int32_t temperature = 0;
    int32_t device_state = 1; // Let's assume "1" for ON and "0" for OFF

    while (1) {
        // Store device state ("1" or "0")
        err = nvs_set_i32(my_handle, STATE_KEY, device_state);
        if (err != ESP_OK) {
            printf("Failed to write device state to NVS (%s)!\n", esp_err_to_name(err));
        }

        // For the sake of the example, we increment the temperature
        temperature++;

        // Store temperature
        err = nvs_set_i32(my_handle, TEMPERATURE_KEY, temperature);
        if (err != ESP_OK) {
            printf("Failed to write temperature to NVS (%s)!\n", esp_err_to_name(err));
        }

        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit changes to NVS (%s)!\n", esp_err_to_name(err));
        }

        printf("Device state: %"PRIi32", Temperature: %"PRIi32"\n", device_state, temperature);

        // Delay for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle (although the loop above does not terminate)
    nvs_close(my_handle);
    return 0;
}
