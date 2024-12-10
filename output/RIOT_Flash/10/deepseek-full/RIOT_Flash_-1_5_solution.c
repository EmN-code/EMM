
#include "esp_system.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
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
#include "esp_wifi.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/sock/udp.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void update_device_state(bool state) {
    esp_err_t err;
    err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state ? 1 : 0);
    if (err != ESP_OK) {
        printf("Error setting device state: %s\n", esp_err_to_name(err));
        return;
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing device state: %s\n", esp_err_to_name(err));
    }
}

int main(void) {
    esp_err_t err;
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing NVS flash: %s\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        bool current_state = (rand() % 2) == 1; // Simulate temperature state
        update_device_state(current_state);
        ztimer_sleep(ZTIMER_MSEC, 1000); // Wait for 1 second
    }

    nvs_close(my_handle);
    return 0;
}
