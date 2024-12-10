
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time_units.h>
#include <xtimer.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <nvs.h>
#include <stddef.h>
#include <mbox.h>

// Function prototypes
void initialize_nvs(void);
void store_state(nvs_handle_t handle, uint8_t state);
uint8_t read_state(nvs_handle_t handle);

int main() {
    nvs_handle_t handle;
    uint8_t state = 0;

    // Initialize NVS
    initialize_nvs();

    // Open NVS storage
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    while (1) {
        // Toggle state
        state = !state;

        // Store the state in NVS
        store_state(handle, state);

        // Read the state from NVS
        uint8_t stored_state = read_state(handle);
        printf("Stored state: %d\n", stored_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}

void initialize_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%d) storing state!\n", err);
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing state!\n", err);
        }
    }
}

uint8_t read_state(nvs_handle_t handle) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(handle, "device_state", &state);
    if (err != ESP_OK) {
        printf("Error (%d) reading state!\n", err);
    }
    return state;
}
