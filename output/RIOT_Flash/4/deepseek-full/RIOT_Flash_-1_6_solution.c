
#include <net/sock/udp.h>
#include <mbox.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <msg.h>
#include <timex.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdatomic.h>
#include <nvs.h>
#include <net/emcute.h>
#include <time_units.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <netinet/in.h>
#include <string.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <ztimer.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void update_device_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting device state: %s\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing device state: %s\n", esp_err_to_name(err));
        }
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

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        uint8_t current_state;
        err = nvs_get_u8(my_handle, DEVICE_STATE_KEY, &current_state);
        if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
            printf("Error reading device state: %s\n", esp_err_to_name(err));
        } else {
            current_state = (current_state == 0) ? 1 : 0;
            update_device_state(current_state);
        }

        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}
