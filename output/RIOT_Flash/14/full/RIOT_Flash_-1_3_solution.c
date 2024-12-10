#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>

#define STORAGE_NAMESPACE "storage"
#define INIT_TEMP_STATE "0"

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        nvs_flash_erase();
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Failed to initialize NVS: %s\n", esp_err_to_name(err));
        exit(EXIT_FAILURE);
    }
}

void save_device_state(nvs_handle_t handle, const char *state) {
    esp_err_t err = nvs_set_str(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error saving state to NVS: %s\n", esp_err_to_name(err));
    } else {
        nvs_commit(handle);
    }
}

int main(void) {
    init_nvs();
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    char state[2];
    size_t required_size = sizeof(state);
    while (1) {
        err = nvs_get_str(nvs_handle, "device_state", state, &required_size);
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            strncpy(state, INIT_TEMP_STATE, sizeof(state));
        } else if (err != ESP_OK) {
            printf("Error (%s) reading NVS!\n", esp_err_to_name(err));
        }

        // Flip the state
        state[0] = (state[0] == '0') ? '1' : '0';
        save_device_state(nvs_handle, state);

        ztimer_sleep(ZTIMER_SEC, 1); // wait for 1 second
    }
    nvs_close(nvs_handle);
    return 0;
}