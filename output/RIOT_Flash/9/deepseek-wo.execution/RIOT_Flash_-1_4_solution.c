
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

void update_device_state(void)
{
    static bool state = false;
    state = !state;

    esp_err_t err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state ? 1 : 0);
    if (err != ESP_OK) {
        printf("Error writing to NVS: %d\n", err);
        return;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing to NVS: %d\n", err);
    }
}

int main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            printf("Error erasing NVS flash: %d\n", err);
            return -1;
        }
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing NVS flash: %d\n", err);
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return -1;
    }

    while (1) {
        update_device_state();
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}
