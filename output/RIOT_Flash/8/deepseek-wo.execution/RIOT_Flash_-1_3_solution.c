
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <xtimer.h>
#include <timex.h>
#include <ztimer.h>
#include <msg.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;

void store_state(uint8_t state) {
    nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    nvs_commit(nvs_handle);
}

uint8_t read_state(void) {
    uint8_t state = 0;
    nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    return state;
}

int main(void) {
    // Initialize NVS
    nvs_flash_init();
    nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);

    while (1) {
        // Simulate temperature update
        uint8_t current_state = read_state();
        printf("Current state: %d\n", current_state);

        // Toggle state
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(new_state);
        printf("Updated state: %d\n", new_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(nvs_handle);

    return 0;
}
