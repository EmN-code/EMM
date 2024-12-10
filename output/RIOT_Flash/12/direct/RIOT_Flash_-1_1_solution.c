#include <stdio.h>
#include "xtimer.h"
#include "nvs.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define INTERVAL (1U * US_PER_SEC)

static void print_current_state(nvs_handle_t nvs_handle) {
    char state_value[2];
    size_t required_size = sizeof(state_value);
    if (nvs_get_str(nvs_handle, STATE_KEY, state_value, &required_size) == 0) {
        printf("Current Device State: %s\n", state_value);
    } else {
        printf("No state found in NVS, initialising to '0'.\n");
        state_value[0] = '0';
        state_value[1] = '\0';
        nvs_set_str(nvs_handle, STATE_KEY, state_value);
        nvs_commit(nvs_handle);
    }
}

int main(void) {
    nvs_flash_init();
    nvs_handle_t nvs_handle;
    if (nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle) != 0) {
        printf("Error opening NVS handle!\n");
        return 1;
    }

    print_current_state(nvs_handle);

    while (1) {
        char state_value[2];
        size_t required_size = sizeof(state_value);
        if (nvs_get_str(nvs_handle, STATE_KEY, state_value, &required_size) == 0) {
            // Toggle state
            state_value[0] = (state_value[0] == '0') ? '1' : '0';
            state_value[1] = '\0';
            nvs_set_str(nvs_handle, STATE_KEY, state_value);
            nvs_commit(nvs_handle);
            printf("Updated Device State to: %s\n", state_value);
        }
        xtimer_sleep(INTERVAL);
    }

    nvs_close(nvs_handle);
    return 0;
}
