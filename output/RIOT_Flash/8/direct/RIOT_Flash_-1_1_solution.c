#include <stdio.h>
#include <stdint.h>
#include "xtimer.h"
#include "nvs.h"

#define INTERVAL (1U * US_PER_SEC)  // Loop interval of 1 second.
#define NVS_SIZE 64  // Memory size for NVS.

int main(void) {
    nvs_t dev;
    nvs_flash_init_partition("nvs_partition", NVS_SIZE, &dev);

    char current_state[2] = "0";

    while (1) {
        // Set a new state value - toggle between "0" and "1"
        if (strcmp(current_state, "0") == 0) {
            strcpy(current_state, "1");
        } else {
            strcpy(current_state, "0");
        }

        // Write the state to NVS
        size_t write_size = strlen(current_state) + 1;  // +1 for the null terminator
        if (nvs_set(dev, "device_state", current_state, write_size) == NVS_NO_ERR) {
            printf("Stored state: %s\n", current_state);
        } else {
            printf("Failed to store the state\n");
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    nvs_flash_erase_partition(&dev);
    return 0;
}