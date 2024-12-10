
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <xtimer.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void store_state(nvs_handle_t handle, uint8_t state) {
    nvs_set_u8(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
}

uint8_t read_state(nvs_handle_t handle) {
    uint8_t state = 0;
    nvs_get_u8(handle, DEVICE_STATE_KEY, &state);
    return state;
}

int main(void) {
    nvs_handle_t handle;
    nvs_flash_init();
    nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);

    while (1) {
        uint8_t current_state = read_state(handle);
        printf("Current state: %d\n", current_state);

        // Toggle state
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(handle, new_state);

        printf("Updated state: %d\n", new_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    nvs_close(handle);
    return 0;
}
