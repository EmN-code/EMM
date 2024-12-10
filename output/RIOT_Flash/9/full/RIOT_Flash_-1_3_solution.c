#include "ztimer.h"
#include "xtimer.h"
#include "unstd.h"
#include "time_units.h"
#include "timex.h"
#include "time.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

static void handle_error(esp_err_t err) {
    if (err != ESP_OK) {
        printf("NVS error: %d\n", err);
        exit(EXIT_FAILURE);
    }
}

void toggle_device_state(void) {
    nvs_handle_t handle;
    esp_err_t err;
    uint8_t state = 0;

    // Retry mechanism for NVS open
    while (true) {
        err = nvs_open("storage", NVS_READWRITE, &handle);
        if (err == ESP_OK) {
            printf("NVS opened successfully.\n");
            break;
        } else {
            printf("Failed to open NVS. Retrying...\n");
            sleep(1);
        }
    }

    // Read the state from NVS
    err = nvs_get_u8(handle, "state", &state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("State not found in NVS, initializing to 0.\n");
        state = 0;
    } else {
        handle_error(err);
    }

    // Toggle the state
    state = !state;
    printf("New device state: %d\n", state);

    // Write the new state to NVS
    err = nvs_set_u8(handle, "state", state);
    handle_error(err);

    // Commit written value
    err = nvs_commit(handle);
    handle_error(err);

    // Close NVS handle
    nvs_close(handle);
}

int main() {
    esp_err_t err;

    // Initialize NVS
    while (true) {
        err = nvs_flash_init();
        if (err == ESP_OK) {
            printf("NVS Flash initialized successfully.\n");
            break;
        } else {
            printf("Failed to initialize NVS Flash. Retrying...\n");
            sleep(1);
        }
    }

    // Main loop for toggling state every second
    while (true) {
        toggle_device_state();
        xtimer_sleep(1);
    }

    return 0;
}