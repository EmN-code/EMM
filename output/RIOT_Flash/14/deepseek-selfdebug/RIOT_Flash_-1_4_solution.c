
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

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
        /* Read the current state from NVS */
        err = nvs_get_i32(my_handle, NVS_KEY, &state);
        if (err != NVS_OK) {
            printf("Error (%d) reading state from NVS!\n", err);
        } else {
            printf("Current state: %d\n", state);
        }

        /* Toggle the state */
        state = (state == 0) ? 1 : 0;

        /* Write the new state to NVS */
        err = nvs_set_i32(my_handle, NVS_KEY, state);
        if (err != NVS_OK) {
            printf("Error (%d) writing state to NVS!\n", err);
        } else {
            printf("New state written: %d\n", state);
        }

        /* Commit the changes to NVS */
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("Error (%d) committing to NVS!\n", err);
        }

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close the NVS handle */
    nvs_close(my_handle);

    return 0;
}
