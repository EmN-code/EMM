#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

#define STORAGE_NAMESPACE "storage"
#define KEY "device_state"

void init_nvs(void);
void store_device_state(nvs_handle_t nvs_handle, int8_t state);
int8_t read_device_state(nvs_handle_t nvs_handle);

int main(void) {
    init_nvs();
    nvs_handle_t nvs_handle;
    nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);

    while (1) {
        // Simulate reading temperature, here we toggle between 0 and 1
        static int8_t state = 0;
        state = !state;
        store_device_state(nvs_handle, state);
        printf("Stored state: %d\n", state);

        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        nvs_flash_erase();
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Failed to initialize NVS\n");
        exit(EXIT_FAILURE);
    }
}

void store_device_state(nvs_handle_t nvs_handle, int8_t state) {
    nvs_set_i8(nvs_handle, KEY, state);
    nvs_commit(nvs_handle);
}

int8_t read_device_state(nvs_handle_t nvs_handle) {
    int8_t state;
    nvs_get_i8(nvs_handle, KEY, &state);
    return state;
}