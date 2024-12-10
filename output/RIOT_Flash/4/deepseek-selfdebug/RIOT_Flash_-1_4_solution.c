
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t state = 0;
    int32_t stored_state = 0;
    nvs_err_t err;

    /* Initialize NVS */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    while (1) {
        /* Toggle state */
        state = (state == 0) ? 1 : 0;

        /* Write state to NVS */
        err = nvs_set_i32(my_handle, NVS_KEY, state);
        if (err != NVS_OK) {
            printf("NVS set failed: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        /* Commit writes */
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        /* Read state from NVS */
        err = nvs_get_i32(my_handle, NVS_KEY, &stored_state);
        if (err != NVS_OK) {
            printf("NVS get failed: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        /* Print the stored state */
        printf("Stored state: %ld\n", (long)stored_state);

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close NVS */
    nvs_close(my_handle);
    return 0;
}
