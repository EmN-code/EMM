
#include <stdio.h>
#include <xtimer.h>
#include <periph/nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t handle;
    int32_t state = 0;
    int err;

    // Initialize NVS
    err = nvs_init();
    if (err != 0) {
        printf("NVS init failed with error %d\n", err);
        return 1;
    }

    // Open NVS namespace
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != 0) {
        printf("NVS open failed with error %d\n", err);
        return 1;
    }

    while (1) {
        // Toggle state
        state = (state == 0) ? 1 : 0;

        // Write state to NVS
        err = nvs_set_i32(handle, NVS_KEY, state);
        if (err != 0) {
            printf("NVS set failed with error %d\n", err);
            return 1;
        }

        // Commit changes to NVS
        err = nvs_commit(handle);
        if (err != 0) {
            printf("NVS commit failed with error %d\n", err);
            return 1;
        }

        // Print current state
        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}
