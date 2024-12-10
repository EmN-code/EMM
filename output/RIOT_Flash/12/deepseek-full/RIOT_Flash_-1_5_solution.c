
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <nvs_flash.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <nvs.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>

#define NVS_PARTITION "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;
static bool device_state = false;

void toggle_state(void) {
    device_state = !device_state;
    int state = device_state ? 1 : 0;
    nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    nvs_commit(nvs_handle);
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init_partition(NVS_PARTITION);
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase_partition(NVS_PARTITION);
        err = nvs_flash_init_partition(NVS_PARTITION);
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS handle
    err = nvs_open(NVS_PARTITION, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        toggle_state();
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }

    // Close NVS handle (this will never be reached in this example)
    nvs_close(nvs_handle);

    return 0;
}
