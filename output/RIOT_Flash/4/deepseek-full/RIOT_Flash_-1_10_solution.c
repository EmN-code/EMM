
#include "net/sock/udp.h"
#include "esp_wifi.h"
#include "mbox.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "esp_system.h"
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

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void update_device_state(void) {
    static uint8_t state = 0;
    state = !state; // Toggle state

    esp_err_t err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting device state: %d\n", err);
        return;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing NVS: %d\n", err);
    } else {
        printf("Device state updated to: %d\n", state);
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing NVS: %d\n", err);
        return 1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return 1;
    }

    while (1) {
        update_device_state();
        xtimer_sleep(1); // Sleep for 1 second
    }

    nvs_close(my_handle);
    return 0;
}
