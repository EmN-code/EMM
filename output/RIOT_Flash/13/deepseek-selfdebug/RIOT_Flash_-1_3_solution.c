
#include "nvs.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define DEVICE_STATE_KEY "device_state"
#define DEVICE_STATE_VALUE "1"

int main(void)
{
    nvs_handle_t my_handle;
    int err;
    char state[2];

    /* Initialize NVS */
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    while (1) {
        /* Simulate temperature update */
        printf("Updating device state...\n");

        /* Write the device state to NVS */
        err = nvs_set_str(my_handle, DEVICE_STATE_KEY, DEVICE_STATE_VALUE);
        if (err != NVS_OK) {
            printf("NVS set failed: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        /* Commit the write operation */
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        /* Read the device state from NVS */
        err = nvs_get_str(my_handle, DEVICE_STATE_KEY, state, sizeof(state));
        if (err != NVS_OK) {
            printf("NVS get failed: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        printf("Device state: %s\n", state);

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close NVS */
    nvs_close(my_handle);

    return 0;
}
