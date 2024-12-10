#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <esp_system.h>
#include <esp_wifi.h>

void init_nvs(void) {
    while (nvs_flash_init() != ESP_OK) {
        printf("Error initializing flash, retrying...\n");
        xtimer_sleep(1);
    }
}

void store_device_state(nvs_handle_t handle, int state) {
    while (1) {
        esp_err_t err = nvs_set_i32(handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error (%s) storing state to NVS, retrying...\n", esp_err_to_name(err));
            xtimer_sleep(1);
            continue;
        }

        err = nvs_commit(handle);
        if (err == ESP_OK) {
            printf("Successfully stored state: %d\n", state);
            break;
        } else {
            printf("Failed to commit state to NVS: (%s), retrying...\n", esp_err_to_name(err));
            xtimer_sleep(1);
        }
    }
}

int main(void) {
    init_nvs();
    nvs_handle_t storage_handle;

    while (nvs_open("storage", NVS_READWRITE, &storage_handle) != ESP_OK) {
        printf("Failed to open NVS handle, retrying...\n");
        xtimer_sleep(1);
    }

    int device_state = 1;
    while (1) {
        store_device_state(storage_handle, device_state);
        device_state = device_state == 1 ? 0 : 1; // Toggle between 0 and 1
        ztimer_sleep(ZTIMER_USEC, 1000000); // sleep for 1 second
    }

    nvs_close(storage_handle);
    return 0;
}