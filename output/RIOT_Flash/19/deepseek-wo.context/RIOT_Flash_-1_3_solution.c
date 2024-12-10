
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t state = 1; // Initial state
    int err;

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return 1;
    }

    while (1) {
        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Write state to NVS
        err = nvs_set_i32(my_handle, NVS_KEY, state);
        if (err != NVS_OK) {
            printf("Error (%d) writing to NVS!\n", err);
            nvs_close(my_handle);
            return 1;
        }

        // Commit writes to NVS
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("Error (%d) committing to NVS!\n", err);
            nvs_close(my_handle);
            return 1;
        }

        // Print current state
        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);
    return 0;
}
