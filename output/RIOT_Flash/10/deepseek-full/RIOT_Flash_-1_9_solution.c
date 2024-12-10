
#include "esp_system.h"
#include "net/gcoap.h"
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

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void store_state(uint8_t state) {
    esp_err_t err;
    err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) setting device state!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing device state!\n", esp_err_to_name(err));
        }
    }
}

uint8_t read_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(my_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading device state!\n", esp_err_to_name(err));
    }
    return state;
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
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }

    // Main loop
    while (1) {
        uint8_t current_state = read_state();
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(new_state);
        printf("Device state updated to: %d\n", new_state);
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
