
#include "mbox.h"
#include "xtimer.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "timex.h"
#include "stddef.h"
#include "unistd.h"
#include "stdio.h"
#include "stdatomic.h"
#include "nvs.h"
#include "time_units.h"
#include "stdlib.h"
#include "string.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void store_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) storing state\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing state\n", esp_err_to_name(err));
        }
    }
}

uint8_t read_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(my_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading state\n", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS\n", esp_err_to_name(err));
        return err;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        uint8_t current_state = read_state();
        printf("Current state: %d\n", current_state);

        // Toggle state
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(new_state);
        printf("New state stored: %d\n", new_state);

        // Sleep for 1 second
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    nvs_close(my_handle);
    return 0;
}
