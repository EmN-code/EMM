
#include "xtimer.h"
#include "nvs.h"
#include "periph/temp.h"
#include "fmt.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define SLEEP_INTERVAL (1 * US_PER_SEC)

int main(void)
{
    nvs_handle_t my_handle;
    int32_t device_state = 0;
    int err;

    /* Initialize NVS */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != 0) {
        puts("NVS open failed");
        return 1;
    }

    while (1) {
        /* Read temperature */
        int32_t temperature = temp_read();

        /* Update device state based on temperature */
        device_state = (temperature > 25) ? 1 : 0;

        /* Store the device state in NVS */
        err = nvs_set_i32(my_handle, NVS_KEY, device_state);
        if (err != 0) {
            puts("NVS set failed");
            nvs_close(my_handle);
            return 1;
        }

        /* Commit the changes to NVS */
        err = nvs_commit(my_handle);
        if (err != 0) {
            puts("NVS commit failed");
            nvs_close(my_handle);
            return 1;
        }

        /* Print the current device state */
        printf("Device state: %" PRId32 "\n", device_state);

        /* Sleep for 1 second */
        xtimer_sleep(SLEEP_INTERVAL);
    }

    /* Close NVS */
    nvs_close(my_handle);

    return 0;
}
