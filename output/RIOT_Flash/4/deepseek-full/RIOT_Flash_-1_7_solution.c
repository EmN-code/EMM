
#include "net/sock/udp.h"
#include "mbox.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "msg.h"
#include "timex.h"
#include "stddef.h"
#include "unistd.h"
#include "stdio.h"
#include "stdatomic.h"
#include "nvs.h"
#include "net/emcute.h"
#include "mutex.h"
#include "time_units.h"
#include "stdlib.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "string.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "stdbool.h"
#include "ztimer.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;

void store_state(uint8_t state) {
    nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    nvs_commit(nvs_handle);
}

uint8_t read_state(void) {
    uint8_t state = 0;
    nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    assert(err == ESP_OK);

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    assert(err == ESP_OK);

    while (1) {
        uint8_t current_state = read_state();
        printf("Current state: %d\n", current_state);

        // Simulate state change (toggle between 1 and 0)
        current_state = (current_state == 1) ? 0 : 1;

        store_state(current_state);
        printf("Updated state: %d\n", current_state);

        xtimer_sleep(1); // Wait for 1 second
    }

    nvs_close(nvs_handle);
    return 0;
}
