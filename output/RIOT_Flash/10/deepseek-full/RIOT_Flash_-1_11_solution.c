
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
#include <mutex.h>
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
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define STATE_KEY "device_state"

static nvs_handle_t nvs_handle;

void store_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(nvs_handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error storing state: %s\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error committing state: %s\n", esp_err_to_name(err));
        }
    }
}

uint8_t read_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(nvs_handle, STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Error reading state: %s\n", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing NVS flash: %s\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        uint8_t current_state = read_state();
        printf("Current state: %d\n", current_state);

        // Toggle state
        current_state = (current_state == 0) ? 1 : 0;
        store_state(current_state);

        xtimer_sleep(1); // Sleep for 1 second
    }

    nvs_close(nvs_handle);
    return 0;
}
