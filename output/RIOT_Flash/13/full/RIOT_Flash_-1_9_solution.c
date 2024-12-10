#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <timex.h>
#include <stdint.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_system.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mbox.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <esp_wifi.h>

#define STORAGE_NAMESPACE "storage"

void store_device_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err;
    while (true) {
        err = nvs_set_u8(handle, "device_state", state);
        if (err == ESP_OK) {
            printf("State %d written successfully\n", state);
            break;
        } else {
            printf("Error writing state: %s, retrying...\n", esp_err_to_name(err));
            ztimer_sleep(ZTIMER_SEC, 1);
        }
    }
}

uint8_t read_device_state(nvs_handle_t handle) {
    esp_err_t err;
    uint8_t state = 0;
    while (true) {
        err = nvs_get_u8(handle, "device_state", &state);
        if (err == ESP_OK) {
            printf("State read successfully: %d\n", state);
            break;
        } else if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("State not found, initializing to 0\n");
            state = 0;
            break;
        } else {
            printf("Error reading state: %s, retrying...\n", esp_err_to_name(err));
            ztimer_sleep(ZTIMER_SEC, 1);
        }
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS: %s\n", esp_err_to_name(err));
        return -1;
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    uint8_t state = read_device_state(my_handle);

    while (true) {
        store_device_state(my_handle, state);

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing NVS: %s\n", esp_err_to_name(err));
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }

        state = (state == 0) ? 1 : 0;
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    nvs_close(my_handle);
    return 0;
}
