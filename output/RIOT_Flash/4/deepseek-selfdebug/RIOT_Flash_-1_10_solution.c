
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define TEMPERATURE_KEY "temperature"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t temp = 1; // Initial temperature state
    int err;

    // Initialize NVS flash
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash init failed: %d\n", err);
        return 1;
    }

    // Open NVS storage
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    while (1) {
        // Toggle temperature state
        temp = (temp == 1) ? 0 : 1;

        // Write temperature state to NVS
        err = nvs_set_i32(my_handle, TEMPERATURE_KEY, temp);
        if (err != ESP_OK) {
            printf("NVS set failed: %d\n", err);
            return 1;
        }

        // Commit the write operation
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %d\n", err);
            return 1;
        }

        // Print the current temperature state
        printf("Temperature state updated to: %d\n", temp);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
