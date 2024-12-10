
#include <stdlib.h>
#include <nvs_flash.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <nvs.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <string.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void update_device_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting device state: %d\n", err);
    } else {
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing device state: %d\n", err);
        }
    }
}

uint8_t get_device_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(my_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error getting device state: %d\n", err);
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            printf("Error erasing NVS flash: %d\n", err);
            return -1;
        }
        err = nvs_flash_init();
        if (err != ESP_OK) {
            printf("Error initializing NVS flash: %d\n", err);
            return -1;
        }
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return -1;
    }

    while (1) {
        uint8_t current_state = get_device_state();
        printf("Current device state: %d\n", current_state);

        // Simulate updating the state (toggle between 1 and 0)
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        update_device_state(new_state);

        // Wait for 1 second before updating again
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}
