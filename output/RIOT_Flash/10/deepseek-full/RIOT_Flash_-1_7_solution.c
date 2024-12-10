
#include "esp_system.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "nvs_flash.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "nvs.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "string.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/sock/udp.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void update_state(bool state) {
    esp_err_t err;
    err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state ? 1 : 0);
    if (err != ESP_OK) {
        printf("Error setting state: %s\n", esp_err_to_name(err));
        return;
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing state: %s\n", esp_err_to_name(err));
    }
}

int main(void) {
    esp_err_t err;
    bool current_state = false;

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
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Toggle state
        current_state = !current_state;
        update_state(current_state);

        // Wait for 1 second
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
