
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <stdatomic.h>
#include <timex.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <nvs_flash.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void store_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
        } else {
            printf("State %u written and committed to NVS\n", state);
        }
    }
}

uint8_t read_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(my_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading from NVS!\n", esp_err_to_name(err));
    } else {
        printf("State %u read from NVS\n", state);
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
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        uint8_t current_state = read_state();
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(new_state);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    nvs_close(my_handle);
    return 0;
}
