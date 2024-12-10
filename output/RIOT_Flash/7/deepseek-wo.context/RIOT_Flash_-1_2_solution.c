
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t state = 0;
    int err;

    /* Initialize NVS */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return 1;
    }

    while (1) {
        /* Toggle the state */
        state = (state == 0) ? 1 : 0;

        /* Write the state to NVS */
        err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, state);
        if (err != NVS_OK) {
            printf("Error (%d) writing to NVS!\n", err);
            nvs_close(my_handle);
            return 1;
        }

        /* Commit the state to NVS */
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("Error (%d) committing to NVS!\n", err);
            nvs_close(my_handle);
            return 1;
        }

        /* Print the current state */
        printf("Device state updated to: %d\n", state);

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close the NVS handle */
    nvs_close(my_handle);

    return 0;
}
