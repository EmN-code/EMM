
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_flash_init();
    nvs_handle_t my_handle;
    int32_t state = 0;
    int err;

    while (1) {
        // Open NVS
        err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) opening NVS handle!\n", err);
        } else {
            // Read the current state from NVS
            err = nvs_get_i32(my_handle, NVS_KEY, &state);
            switch (err) {
                case ESP_OK:
                    printf("State read from NVS: %d\n", state);
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    printf("The value is not initialized yet!\n");
                    break;
                default:
                    printf("Error (%d) reading from NVS!\n", err);
            }

            // Toggle the state
            state = (state == 0) ? 1 : 0;

            // Write the new state to NVS
            err = nvs_set_i32(my_handle, NVS_KEY, state);
            printf((err != ESP_OK) ? "State update failed!\n" : "State updated!\n");

            // Commit the changes to NVS
            err = nvs_commit(my_handle);
            printf((err != ESP_OK) ? "State commit failed!\n" : "State committed!\n");

            // Close NVS
            nvs_close(my_handle);
        }

        // Wait for 1 second
        xtimer_sleep(1);
    }

    return 0;
}
