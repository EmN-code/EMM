
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <timex.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <mbox.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <ztimer.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_PARTITION_LABEL "storage"
#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

static atomic_int device_state = 0;

void update_state(void)
{
    device_state = !device_state;
    printf("Device state updated to: %d\n", device_state);
}

void store_state(void)
{
    nvs_handle_t handle;
    esp_err_t err;

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    err = nvs_set_u8(handle, NVS_KEY, (uint8_t)device_state);
    if (err != ESP_OK) {
        printf("Error (%s) storing state!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing state!\n", esp_err_to_name(err));
        }
    }

    nvs_close(handle);
}

void load_state(void)
{
    nvs_handle_t handle;
    esp_err_t err;
    uint8_t state;

    err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    err = nvs_get_u8(handle, NVS_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) loading state!\n", esp_err_to_name(err));
    } else {
        device_state = state;
        printf("Device state loaded: %d\n", device_state);
    }

    nvs_close(handle);
}

int main(void)
{
    esp_err_t err;

    err = nvs_flash_init_partition(NVS_PARTITION_LABEL);
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS partition!\n", esp_err_to_name(err));
        return 1;
    }

    load_state();

    while (1) {
        update_state();
        store_state();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
