
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <ztimer.h>
#include <msg.h>
#include <esp_system.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <stdbool.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <mbox.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <mutex.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return;
    }
}

void store_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("NVS set failed: %d\n", err);
        return;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("NVS commit failed: %d\n", err);
    }
}

uint8_t read_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("NVS get failed: %d\n", err);
    }
    return state;
}

int main(void) {
    init_nvs();

    while (1) {
        uint8_t current_state = read_state();
        printf("Current state: %d\n", current_state);

        // Simulate updating the state
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(new_state);
        printf("Updated state: %d\n", new_state);

        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    nvs_close(nvs_handle);
    return 0;
}
