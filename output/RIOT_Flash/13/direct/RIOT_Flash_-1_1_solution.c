#include "xtimer.h"
#include "nvs.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define DELAY_US (1U * US_PER_SEC)  // 1 second delay
#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static nvs_t nvs;
static uint8_t device_state = 0; // initial state

void store_device_state(uint8_t state) {
    int res = nvs_set(&nvs, NVS_KEY, &state, sizeof(state));
    if (res < 0) {
        printf("Error storing device state\n");
    } else {
        printf("Device state stored: %d\n", state);
    }
}

void init_storage(void) {
    if (nvs_init(&nvs, NVS_NAMESPACE) < 0) {
        printf("Failed to initialize NVS\n");
    }
}

int main(void) {
    // Initialize storage
    init_storage();

    while (1) {
        // Store the current state
        store_device_state(device_state);

        // Toggle the device state for demonstration
        device_state = !device_state;

        // Wait before storing the next state
        xtimer_usleep(DELAY_US);
    }

    return 0;
}