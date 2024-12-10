
#include "xtimer.h"
#include "nvs.h"
#include "periph/gpio.h"
#include "fmt.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void update_device_state(nvs_handle_t handle, int32_t state) {
    int err;
    err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    if (err != NVS_OK) {
        puts("Error setting device state in NVS");
        return;
    }
    err = nvs_commit(handle);
    if (err != NVS_OK) {
        puts("Error committing device state to NVS");
    }
}

int main(void) {
    nvs_handle_t handle;
    int err;
    int32_t device_state = 0;

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != NVS_OK) {
        puts("Error opening NVS");
        return 1;
    }

    while (1) {
        // Simulate temperature update logic here
        device_state = (device_state == 0) ? 1 : 0; // Toggle state

        // Update device state in NVS
        update_device_state(handle, device_state);

        // Print the current state
        printf("Device state updated to: %" PRId32 "\n", device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    nvs_close(handle);
    return 0;
}
