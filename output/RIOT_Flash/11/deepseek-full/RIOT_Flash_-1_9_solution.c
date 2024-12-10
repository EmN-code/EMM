
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdatomic.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static mutex_t nvs_mutex = MUTEX_INIT;

void store_device_state(nvs_handle_t handle, uint8_t state) {
    mutex_lock(&nvs_mutex);
    nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
    mutex_unlock(&nvs_mutex);
}

uint8_t read_device_state(nvs_handle_t handle) {
    uint8_t state = 0;
    mutex_lock(&nvs_mutex);
    nvs_get_u8(handle, DEVICE_STATE_KEY, &state);
    mutex_unlock(&nvs_mutex);
    return state;
}

int main(void) {
    esp_err_t err;
    nvs_handle_t handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    ESP_ERROR_CHECK(err);

    while (1) {
        // Simulate temperature update
        uint8_t current_state = read_device_state(handle);
        current_state = (current_state == 1) ? 0 : 1; // Toggle state
        store_device_state(handle, current_state);

        // Print current state
        printf("Device state updated to: %d\n", current_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(handle);

    return 0;
}
