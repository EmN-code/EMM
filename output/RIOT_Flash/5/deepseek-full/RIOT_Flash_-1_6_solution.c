
#include <net/sock/udp.h>
#include <mbox.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <msg.h>
#include <timex.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdatomic.h>
#include <nvs.h>
#include <net/emcute.h>
#include <time_units.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <netinet/in.h>
#include <string.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <ztimer.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void update_device_state(uint8_t state) {
    nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    nvs_commit(my_handle);
}

uint8_t get_device_state(void) {
    uint8_t state;
    nvs_get_u8(my_handle, DEVICE_STATE_KEY, &state);
    return state;
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    assert(err == ESP_OK);

    // Open NVS handle
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    assert(err == ESP_OK);

    while (1) {
        // Simulate temperature update
        uint8_t current_state = get_device_state();
        printf("Current Device State: %d\n", current_state);

        // Toggle state
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        update_device_state(new_state);
        printf("Updated Device State: %d\n", new_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
