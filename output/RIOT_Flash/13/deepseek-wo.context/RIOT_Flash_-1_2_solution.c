
#include "nvs.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "fmt.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static nvs_handle_t nvs_handle;

void update_device_state(int state) {
    int err;
    err = nvs_set_u8(nvs_handle, NVS_KEY, state);
    if (err != NVS_OK) {
        printf("Error setting NVS value: %d\n", err);
    }
    err = nvs_commit(nvs_handle);
    if (err != NVS_OK) {
        printf("Error committing NVS value: %d\n", err);
    }
}

int main(void) {
    int err;
    uint8_t state;

    // Initialize NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != NVS_OK) {
        printf("Error opening NVS: %d\n", err);
        return 1;
    }

    // Main loop to update device state every second
    while (1) {
        // Simulate getting the device state (e.g., from a sensor)
        state = (state == 1) ? 0 : 1; // Toggle state

        // Update the device state in NVS
        update_device_state(state);

        // Print the current state
        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS (this will never be reached in this example)
    nvs_close(nvs_handle);

    return 0;
}
