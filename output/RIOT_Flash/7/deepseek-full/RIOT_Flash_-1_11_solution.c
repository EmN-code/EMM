
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

#define DEVICE_STATE_KEY "device_state"

static mutex_t state_mutex = MUTEX_INIT;
static nvs_handle_t nvs_handle;

void update_state(void)
{
    uint8_t state = 0;
    esp_err_t err;

    mutex_lock(&state_mutex);

    // Read the current state from NVS
    err = nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Failed to read state from NVS: %s\n", esp_err_to_name(err));
        mutex_unlock(&state_mutex);
        return;
    }

    // Toggle the state
    state = (state == 0) ? 1 : 0;

    // Write the new state to NVS
    err = nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to write state to NVS: %s\n", esp_err_to_name(err));
        mutex_unlock(&state_mutex);
        return;
    }

    // Commit the changes to NVS
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to commit state to NVS: %s\n", esp_err_to_name(err));
    }

    mutex_unlock(&state_mutex);
}

int main(void)
{
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        update_state();
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }

    // Close NVS handle
    nvs_close(nvs_handle);

    return 0;
}
