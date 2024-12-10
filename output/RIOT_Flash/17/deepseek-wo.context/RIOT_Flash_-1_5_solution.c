
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <xtimer.h>
#include "periph/nvs.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(uint8_t state) {
    nvs_handle_t handle;
    int err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != 0) {
        printf("Error opening NVS handle: %d\n", err);
        return;
    }

    err = nvs_set(handle, NVS_KEY, &state, sizeof(state));
    if (err != 0) {
        printf("Error writing to NVS: %d\n", err);
    } else {
        printf("State stored: %d\n", state);
    }

    nvs_close(handle);
}

int main(void) {
    uint8_t state = 1;

    while (1) {
        store_state(state);
        state = (state == 1) ? 0 : 1; // Toggle state
        xtimer_sleep(1); // Wait for 1 second
    }

    return 0;
}
