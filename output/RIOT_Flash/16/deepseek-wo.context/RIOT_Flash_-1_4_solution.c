
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_flash_init();
    nvs_handle_t my_handle;
    int32_t state = 1;
    int err;

    while (1) {
        // Open NVS
        err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) opening NVS handle!\n", err);
        } else {
            // Write state to NVS
            err = nvs_set_i32(my_handle, NVS_KEY, state);
            if (err != ESP_OK) {
                printf("Error (%d) writing to NVS!\n", err);
            } else {
                printf("State %d written to NVS\n", state);
            }

            // Commit written value.
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%d) committing to NVS!\n", err);
            }

            // Close NVS
            nvs_close(my_handle);
        }

        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Wait for 1 second
        xtimer_sleep(1);
    }

    return 0;
}
