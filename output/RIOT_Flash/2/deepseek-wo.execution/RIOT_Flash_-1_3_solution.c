
#include <string.h>
#include <stddef.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <stdatomic.h>
#include <nvs_flash.h>
#include <stdlib.h>
#include <nvs.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <stdio.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;

void store_device_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) setting device state!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing device state!\n", esp_err_to_name(err));
        }
    }
}

uint8_t read_device_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading device state!\n", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        uint8_t current_state = read_device_state();
        printf("Current device state: %d\n", current_state);

        // Toggle the state
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_device_state(new_state);
        printf("Updated device state: %d\n", new_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}
