#include <stdio.h>
#include "xtimer.h"
#include "nvs.h"

#define SLEEP_INTERVAL (1000000)  // Sleep interval of 1 second

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t temperature = 0;

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    } else {
        printf("NVS open OK\n");
    }

    while (1) {
        // Simulate a temperature read
        temperature = (temperature == 0) ? 1 : 0;

        // Write temperature to NVS
        err = nvs_set_u8(my_handle, "temperature", temperature);
        if (err != ESP_OK) {
            printf("Failed to write temperature to NVS (%s)\n", esp_err_to_name(err));
        }
        // Commit the updated value to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit temperature to NVS (%s)\n", esp_err_to_name(err));
        }

        xtimer_sleep(1);
    }

    // Close NVS handle (unreachable code in this example)
    nvs_close(my_handle);
    return 0;
}