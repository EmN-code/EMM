#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "nvs.h"
#include "msg.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define INTERVAL (1U * US_PER_SEC) // 1 second

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Open Non-Volatile Storage with a given namespace
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", (int)err);
        return err;
    }

    while (1) {
        // Simulate reading a temperature or state
        int temp = 1; // Simplified for demonstration purposes

        // Write the value to NVS
        err = nvs_set_i32(my_handle, NVS_KEY, temp);
        if (err != ESP_OK) {
            printf("Error (%d) writing to NVS!\n", (int)err);
        }

        // Commit the written value to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed with error (%d)!\n", (int)err);
        }

        // Wait for interval before updating again
        xtimer_usleep(INTERVAL);
    }

    // Close NVS handle - typically unreachable code in this looping example
    nvs_close(my_handle);
    return 0;
}