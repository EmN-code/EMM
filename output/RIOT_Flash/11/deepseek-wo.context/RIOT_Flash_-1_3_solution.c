
#include "nvs.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int8_t state = 0;
    int err;

    /* Initialize NVS */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    while (1) {
        /* Toggle state */
        state = (state == 0) ? 1 : 0;

        /* Store the state in NVS */
        err = nvs_set_i8(my_handle, NVS_KEY, state);
        if (err != NVS_OK) {
            printf("NVS set failed: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        /* Commit the changes to NVS */
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        /* Print the current state */
        printf("Device state updated to: %d\n", state);

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close NVS handle */
    nvs_close(my_handle);

    return 0;
}