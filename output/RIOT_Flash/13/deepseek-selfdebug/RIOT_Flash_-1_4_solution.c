
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_flash_init();

    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    while (1) {
        // Simulate temperature update
        int temperature = (rand() % 100) < 50 ? 1 : 0;
        char state[2];
        snprintf(state, sizeof(state), "%d", temperature);

        // Store the state in NVS
        err = nvs_set_str(my_handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("Error (%d) setting NVS value!\n", err);
            nvs_close(my_handle);
            return err;
        }

        // Commit the changes
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing NVS value!\n", err);
            nvs_close(my_handle);
            return err;
        }

        // Print the stored state
        char stored_state[2];
        size_t length = sizeof(stored_state);
        err = nvs_get_str(my_handle, NVS_KEY, stored_state, &length);
        if (err != ESP_OK) {
            printf("Error (%d) getting NVS value!\n", err);
            nvs_close(my_handle);
            return err;
        }
        printf("Stored state: %s\n", stored_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close the NVS handle
    nvs_close(my_handle);
    return 0;
}
