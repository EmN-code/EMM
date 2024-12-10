
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdatomic.h>
#include <time_units.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <ztimer.h>
#include <xtimer.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <mutex.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <timex.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <mbox.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;
static bool device_state = false;

void toggle_state(void) {
    device_state = !device_state;
    int8_t state = device_state ? 1 : 0;
    nvs_set_i8(nvs_handle, DEVICE_STATE_KEY, state);
    nvs_commit(nvs_handle);
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    // Main loop
    while (1) {
        toggle_state();
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(nvs_handle);
    return 0;
}
