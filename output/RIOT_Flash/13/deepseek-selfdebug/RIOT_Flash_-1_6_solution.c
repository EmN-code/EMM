
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define NVS_VALUE_SIZE 2

int main(void)
{
    nvs_handle_t my_handle;
    int err;
    char state[NVS_VALUE_SIZE];

    // Initialize NVS
    err = nvs_flash_init();
    if (err != NVS_OK) {
        printf("NVS Flash initialization failed: %d\n", err);
        return 1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    while (1) {
        // Simulate temperature update (toggle state between "1" and "0")
        static int current_state = 0;
        current_state = (current_state + 1) % 2;
        snprintf(state, NVS_VALUE_SIZE, "%d", current_state);

        // Write state to NVS
        err = nvs_set_str(my_handle, NVS_KEY, state);
        if (err != NVS_OK) {
            printf("NVS set failed: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        // Commit the write operation
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        printf("Device state updated to: %s\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
