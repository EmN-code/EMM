
#include "nvs.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "fmt.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int err;
    uint8_t state = 0;

    /* Initialize NVS */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        puts("NVS open failed");
        return 1;
    }

    while (1) {
        /* Toggle the state */
        state = (state == 0) ? 1 : 0;

        /* Write the state to NVS */
        err = nvs_set_u8(my_handle, NVS_KEY, state);
        if (err != NVS_OK) {
            puts("NVS set failed");
            nvs_close(my_handle);
            return 1;
        }

        /* Commit the write */
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            puts("NVS commit failed");
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
