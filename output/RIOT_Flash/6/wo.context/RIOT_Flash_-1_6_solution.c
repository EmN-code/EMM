#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"
#define TEMPERATURE_KEY "temperature"

void write_state_nvs(nvs_handle_t handle, int state) {
    // Store device state in NVS
    nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
}

void write_temperature_nvs(nvs_handle_t handle, int temperature) {
    // Store temperature in NVS
    nvs_set_i32(handle, TEMPERATURE_KEY, temperature);
    nvs_commit(handle);
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS!");
        return -1;
    }

    nvs_handle_t nvs_handle;
    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return -1;
    }

    int temperature = 0;
    int state = 1; // Example initial state

    while (1) {
        // Simulate a temperature reading
        temperature++;

        // Write data to NVS
        write_state_nvs(nvs_handle, state);
        write_temperature_nvs(nvs_handle, temperature);

        printf("State and Temperature updated in NVS: %d, %d\n", state, temperature);

        // Switch device state every iteration
        state = !state;

        // Wait for one second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(nvs_handle);

    return 0;
}