
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t state = 0;
    int err;

    /* Initialize NVS flash */
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS flash init failed: %d\n", err);
        return 1;
    }

    /* Open NVS namespace */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    while (1) {
        /* Toggle the state */
        state = (state == 0) ? 1 : 0;

        /* Write the state to NVS */
        err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, state);
        if (err != ESP_OK) {
            printf("NVS set failed: %d\n", err);
            return 1;
        }

        /* Commit the changes to NVS */
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %d\n", err);
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