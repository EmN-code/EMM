
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void update_device_state(nvs_handle_t handle, int state) {
    int err;
    err = nvs_set_u8(handle, NVS_KEY, state);
    if (err != NVS_OK) {
        printf("Error setting NVS value: %d\n", err);
        return;
    }
    err = nvs_commit(handle);
    if (err != NVS_OK) {
        printf("Error committing NVS value: %d\n", err);
    }
}

int main(void) {
    nvs_handle_t handle;
    int err;
    int state = 0;

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != NVS_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return 1;
    }

    while (1) {
        state = !state;  // Toggle state between 0 and 1
        update_device_state(handle, state);
        printf("Device state updated to: %d\n", state);
        xtimer_sleep(1);  // Wait for 1 second
    }

    nvs_close(handle);
    return 0;
}
