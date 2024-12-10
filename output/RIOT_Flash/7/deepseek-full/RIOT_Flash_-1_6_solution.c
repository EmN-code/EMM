
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <net/gcoap.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdatomic.h>
#include <time_units.h>
#include <kernel_defines.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;

void periodic_task(void) {
    uint8_t state = 0;
    nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    state = (state == 0) ? 1 : 0; // Toggle state
    nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    nvs_commit(nvs_handle);
    printf("Device state updated to: %d\n", state);
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        periodic_task();
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    // Close NVS
    nvs_close(nvs_handle);
    return 0;
}
