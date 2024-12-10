
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <mutex.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <nvs_flash.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <nvs.h>
#include <mbox.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <esp_wifi.h>
#include <unistd.h>
#include <stdatomic.h>
#include <net/sock/udp.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static mutex_t nvs_mutex = MUTEX_INIT;

void update_state(nvs_handle_t handle, uint8_t state) {
    mutex_lock(&nvs_mutex);
    nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
    mutex_unlock(&nvs_mutex);
}

uint8_t read_state(nvs_handle_t handle) {
    uint8_t state = 0;
    mutex_lock(&nvs_mutex);
    nvs_get_u8(handle, DEVICE_STATE_KEY, &state);
    mutex_unlock(&nvs_mutex);
    return state;
}

int main(void) {
    nvs_handle_t handle;
    esp_err_t err;

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
        uint8_t current_state = read_state(handle);
        printf("Current state: %d\n", current_state);

        // Toggle state
        uint8_t new_state = current_state == 1 ? 0 : 1;
        update_state(handle, new_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}
