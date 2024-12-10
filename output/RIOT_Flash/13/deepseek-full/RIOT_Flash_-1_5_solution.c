
#include "esp_system.h"
#include "net/gcoap.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "nvs_flash.h"
#include "timex.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "nvs.h"
#include "mbox.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "xtimer.h"
#include "string.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/sock/udp.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static nvs_handle_t nvs_handle;

void toggle_state(void) {
    int32_t state;
    esp_err_t err = nvs_get_i32(nvs_handle, NVS_KEY, &state);
    if (err != ESP_OK) {
        printf("Error reading state from NVS: %s\n", esp_err_to_name(err));
        return;
    }

    state = (state == 0) ? 1 : 0;

    err = nvs_set_i32(nvs_handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Error writing state to NVS: %s\n", esp_err_to_name(err));
        return;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Error committing state to NVS: %s\n", esp_err_to_name(err));
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing NVS: %s\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        toggle_state();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    nvs_close(nvs_handle);
    return 0;
}
