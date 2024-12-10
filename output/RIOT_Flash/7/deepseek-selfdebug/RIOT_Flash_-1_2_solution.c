
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, int state) {
    char state_str[2];
    sprintf(state_str, "%d", state);
    nvs_set_str(handle, NVS_KEY, state_str);
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t handle;
    int err;

    // Initialize NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != NVS_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    while (1) {
        // Simulate temperature update
        int state = (xtimer_now_usec() % 2); // Alternates between 0 and 1

        // Store the state in NVS
        store_state(handle, state);

        // Print the state for verification
        printf("Stored state: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}
