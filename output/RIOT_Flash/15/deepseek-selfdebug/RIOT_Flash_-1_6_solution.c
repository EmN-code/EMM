
#include "nvs.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "fmt.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int err;
    char state[2];

    /* Initialize NVS */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        puts("NVS open failed");
        return 1;
    }

    while (1) {
        /* Simulate temperature update */
        int temperature = (gpio_read(GPIO_PIN(0, 0)) == GPIO_HIGH) ? 1 : 0;

        /* Convert temperature to string */
        int res = fmt_s16_dec(state, temperature);
        state[res] = '\0';

        /* Write temperature to NVS */
        err = nvs_set_str(my_handle, NVS_KEY, state);
        if (err != NVS_OK) {
            puts("NVS set failed");
            nvs_close(my_handle);
            return 1;
        }

        /* Commit the write operation */
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            puts("NVS commit failed");
            nvs_close(my_handle);
            return 1;
        }

        /* Print the stored state */
        printf("Device state stored: %s\n", state);

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close NVS handle */
    nvs_close(my_handle);

    return 0;
}
