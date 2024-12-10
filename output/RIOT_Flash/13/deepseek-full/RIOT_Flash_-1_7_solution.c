
#include <stdio.h>
#include <unistd.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <esp_system.h>
#include <stddef.h>
#include <sys/socket.h>
#include <esp_wifi.h>
#include <time_units.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <stdlib.h>
#include <xtimer.h>
#include <string.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <mbox.h>
#include <net/emcute.h>
#include <stdbool.h>
#include <mutex.h>
#include <msg.h>
#include <arpa/inet.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void toggle_state(nvs_handle_t handle) {
    int32_t state;
    esp_err_t err = nvs_get_i32(handle, NVS_KEY, &state);
    if (err != ESP_OK) {
        state = 0; // Default state
    }
    state = (state == 0) ? 1 : 0; // Toggle state
    nvs_set_i32(handle, NVS_KEY, state);
    nvs_commit(handle);
}

int main() {
    esp_err_t err;
    nvs_handle_t handle;

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

    // Open NVS namespace
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        toggle_state(handle);
        xtimer_sleep(1); // Sleep for 1 second
    }

    nvs_close(handle);
    return 0;
}
