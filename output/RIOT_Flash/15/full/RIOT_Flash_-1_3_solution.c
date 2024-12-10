#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define STORAGE_NAMESPACE "storage"

void toggle_device_state(nvs_handle_t handle) {
    uint8_t state = 0;
    esp_err_t err;

    // Read the current state
    err = nvs_get_u8(handle, "device_state", &state);
    if (err == ESP_OK) {
        printf("Current state: %d\n", state);
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("State not found in NVS, initializing to 0.\n");
    } else {
        printf("Error (%s) reading!
", esp_err_to_name(err));
    }

    // Toggle the state
    state = !state;
    printf("Toggling state to: %d\n", state);

    // Save the new state
    err = nvs_set_u8(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
    }

    // Commit written value.
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing!
", esp_err_to_name(err));
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated
        // and needs to be erased
        nvs_flash_erase();
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Failed to initialize NVS (%s)\n", esp_err_to_name(err));
        return -1;
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!
", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        toggle_device_state(my_handle);
        ztimer_sleep(ZTIMER_USEC, 1000000UL);  // Sleep for 1 second
    }

    nvs_close(my_handle);
    return 0;
}