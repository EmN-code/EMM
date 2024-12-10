
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t device_state = 0;
    int err;

    /* Initialize NVS */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    while (1) {
        /* Toggle device state */
        device_state = (device_state == 0) ? 1 : 0;

        /* Write device state to NVS */
        err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, device_state);
        if (err != ESP_OK) {
            printf("Error (%d) writing to NVS!\n", err);
            nvs_close(my_handle);
            return err;
        }

        /* Commit written value. This may take time! */
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) commiting to NVS!\n", err);
            nvs_close(my_handle);
            return err;
        }

        /* Print the current device state */
        printf("Device state updated to: %d\n", (int)device_state);

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close NVS handle */
    nvs_close(my_handle);

    return 0;
}
