
#include <esp_system.h>
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <nvs_flash.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <nvs.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <string.h>
#include <esp_wifi.h>
#include <unistd.h>
#include <stdatomic.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;

void store_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error storing state: %s\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error committing state: %s\n", esp_err_to_name(err));
        }
    }
}

uint8_t read_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error reading state: %s\n", esp_err_to_name(err));
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
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        uint8_t current_state = read_state();
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(new_state);
        printf("State updated to: %d\n", new_state);
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}