
#include "esp_system.h"
#include "stddef.h"
#include "evtimer_mbox.h"
#include "nvs_flash.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "nvs.h"
#include "stdio.h"
#include "xtimer.h"
#include "string.h"
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
        state = 0;
    }
    state = (state == 0) ? 1 : 0;
    nvs_set_i32(nvs_handle, NVS_KEY, state);
    nvs_commit(nvs_handle);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        return -1;
    }

    while (1) {
        toggle_state();
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}
