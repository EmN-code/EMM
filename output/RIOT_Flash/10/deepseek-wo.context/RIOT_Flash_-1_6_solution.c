
#include "xtimer.h"
#include "nvs.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define SLEEP_SECONDS 1

static nvs_handle_t nvs_handle;

void update_device_state(int state) {
    int err;
    err = nvs_set_u8(nvs_handle, "device_state", state);
    if (err != NVS_OK) {
        printf("Error setting device state: %d\n", err);
    } else {
        err = nvs_commit(nvs_handle);
        if (err != NVS_OK) {
            printf("Error committing device state: %d\n", err);
        }
    }
}

int main(void) {
    int err;
    uint8_t state = 0;

    // Initialize NVS
    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != NVS_OK) {
        printf("Error opening NVS: %d\n", err);
        return 1;
    }

    // Main loop
    while (1) {
        // Toggle state
        state = !state;

        // Update device state in NVS
        update_device_state(state);

        // Sleep for 1 second
        xtimer_sleep(SLEEP_SECONDS);
    }

    // Close NVS
    nvs_close(nvs_handle);

    return 0;
}
