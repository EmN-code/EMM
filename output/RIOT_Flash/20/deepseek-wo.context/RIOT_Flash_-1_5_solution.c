
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

    /* Initialize NVS flash */
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS flash init failed: %d\n", err);
        return 1;
    }

    /* Erase NVS flash */
    err = nvs_flash_erase();
    if (err != ESP_OK) {
        printf("NVS flash erase failed: %d\n", err);
        return 1;
    }

    /* Open NVS namespace */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    while (1) {
        /* Toggle device state */
        device_state = (device_state == 0) ? 1 : 0;

        /* Store device state in NVS */
        err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, device_state);
        if (err != ESP_OK) {
            printf("NVS set failed: %d\n", err);
            return 1;
        }

        /* Commit changes to NVS */
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %d\n", err);
            return 1;
        }

        /* Print current device state */
        printf("Device state updated: %ld\n", (long)device_state);

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close NVS handle */
    nvs_close(my_handle);

    return 0;
}
