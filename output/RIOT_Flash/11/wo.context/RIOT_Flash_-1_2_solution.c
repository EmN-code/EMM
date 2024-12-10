#include <thread.h>
#include <stdio.h>
#include "esp_nvs.h"

// Function prototypes
void *update_state(void *arg);

char stack[THREAD_STACKSIZE_MAIN];  // Stack for thread

void *update_state(void *arg) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else {
        // Loop to update state every second
        while (1) {
            uint8_t state = 1;
            printf("Updating state to: %" PRIu8 "\n", state);
            err = nvs_set_u8(my_handle, "state", state);
            if (err != ESP_OK) {
                printf("Failed to write state!\n");
            }
            nvs_commit(my_handle);
            xtimer_sleep(1);
        }
    }
    // Close NVS handle
    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, update_state, NULL, "state_updater");
    return 0;
}