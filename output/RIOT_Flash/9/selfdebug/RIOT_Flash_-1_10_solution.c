#include <xtimer.h> // Required for timing functionality
#include <stdio.h> // For input/output functions like printf
#include <stdint.h> // For fixed-width integer types
#include <stdlib.h> // For general utility functions
#include <string.h> // For handling strings
#include <nvs_flash.h> // For NVS functions
#include <nvs.h> // For NVS overall control
#include "random.h" // Ensure random functions are properly defined

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "dev_state"

static nvs_handle_t nvs_handle;

static void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash init failed!\n");
    }

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    }
}

static void store_device_state(uint32_t state) {
    esp_err_t err = nvs_set_u32(nvs_handle, DEVICE_STATE_KEY, state);
    if (err == ESP_OK) {
        err = nvs_commit(nvs_handle);
        printf("Device state saved: %lu\n", state);
    } else {
        printf("Failed to save device state!\n");
    }
}

static void simulate_temperature_update(void) {
    uint32_t random_val = random_uint32_range(0, 1); // Simulates temperature reading
    store_device_state(random_val);
}

int main(void) {
    init_nvs();

    while (1) {
        simulate_temperature_update();
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}