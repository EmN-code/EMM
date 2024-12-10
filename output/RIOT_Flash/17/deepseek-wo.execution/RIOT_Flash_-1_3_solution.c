
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time_units.h>
#include <xtimer.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <nvs.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;

void store_device_state(bool state) {
    esp_err_t err;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    err = nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state ? 1 : 0);
    if (err != ESP_OK) {
        printf("Error (%s) setting device state!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing NVS handle!\n", esp_err_to_name(err));
        }
    }

    nvs_close(nvs_handle);
}

bool read_device_state(void) {
    esp_err_t err;
    uint8_t state = 0;

    err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        printf("Error (%s) reading device state!\n", esp_err_to_name(err));
    }

    nvs_close(nvs_handle);
    return state != 0;
}

int main(void) {
    esp_err_t err;

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            printf("Error (%s) erasing NVS partition!\n", esp_err_to_name(err));
            return -1;
        }
        err = nvs_flash_init();
        if (err != ESP_OK) {
            printf("Error (%s) initializing NVS partition!\n", esp_err_to_name(err));
            return -1;
        }
    }

    while (1) {
        bool current_state = read_device_state();
        printf("Current device state: %s\n", current_state ? "ON" : "OFF");

        // Toggle the state
        current_state = !current_state;
        store_device_state(current_state);

        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}
