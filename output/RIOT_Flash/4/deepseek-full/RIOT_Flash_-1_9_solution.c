
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

static nvs_handle_t nvs_handle;

void store_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) storing state\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing state\n", esp_err_to_name(err));
        }
    }
}

uint8_t read_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading state\n", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            printf("Error (%s) erasing NVS\n", esp_err_to_name(err));
            return err;
        }
        err = nvs_flash_init();
        if (err != ESP_OK) {
            printf("Error (%s) initializing NVS\n", esp_err_to_name(err));
            return err;
        }
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        uint8_t current_state = read_state();
        printf("Current state: %d\n", current_state);

        // Simulate temperature update
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(new_state);
        printf("Updated state: %d\n", new_state);

        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}
