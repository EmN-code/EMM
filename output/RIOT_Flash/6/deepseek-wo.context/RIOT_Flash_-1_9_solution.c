
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int err;
    int device_state = 1; // Initial state

    // Open the NVS namespace
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return 1;
    }

    while (1) {
        // Toggle the device state
        device_state = (device_state == 1) ? 0 : 1;

        // Store the device state in NVS
        err = nvs_set_u8(my_handle, NVS_KEY, device_state);
        if (err != NVS_OK) {
            printf("Error (%d) setting NVS value!\n", err);
            nvs_close(my_handle);
            return 1;
        }

        // Commit the changes to NVS
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("Error (%d) committing NVS value!\n", err);
            nvs_close(my_handle);
            return 1;
        }

        printf("Device state updated to: %d\n", device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close the NVS handle
    nvs_close(my_handle);

    return 0;
}
