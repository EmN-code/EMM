#include "nvs_flash.h"
#include "nvs.h"
#include <ztimer.h>
#include <xtimer.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <mbox.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void initialize_nvs()
{
    esp_err_t err = nvs_flash_init();
    while (err != ESP_OK) {
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            // NVS partition was truncated and needs to be erased
            err = nvs_flash_erase();
        }
        err = nvs_flash_init();
    }
}

void store_device_state(uint8_t state)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        // Try opening NVS until successful
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        xtimer_sleep(1);
    }

    err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) setting state!\n", esp_err_to_name(err));
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing state!\n", esp_err_to_name(err));
    }

    nvs_close(my_handle);
}

int main(void)
{
    initialize_nvs();
    uint8_t state = 0;

    while (1) {
        store_device_state(state);
        printf("Stored device state: %d\n", state);

        // Toggle state between 0 and 1
        state = (state == 0) ? 1 : 0;

        // Sleep for 1 second
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return 0;
}