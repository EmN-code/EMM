#include <timex.h>
#include <timer.h>
#include <thread.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

// Global variables
static nvs_handle_t nvs_handle;
static const char *storage_name = "storage";
static const char *state_key = "device_state";
static int state = 0;

void update_state(void) {
    // Toggle state
    state = 1 - state;

    // Store state in NVS
    esp_err_t err = nvs_set_i32(nvs_handle, state_key, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    } else {
        printf("Stored state: %d\n", state);
        // Commit written value to NVS
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
        }
    }
}

void timer_callback(void *arg) {
    (void) arg; // Unused
    update_state();
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS (%s)\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS storage handle
    err = nvs_open(storage_name, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }

    // Read the current state from NVS
    err = nvs_get_i32(nvs_handle, state_key, &state);
    switch (err) {
        case ESP_OK:
            printf("Stored initial state: %d\n", state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The state is not initialized yet, setting default state to 0.\n");
            state = 0;
            update_state();
            break;
        default:
            printf("Error (%s) reading from NVS!\n", esp_err_to_name(err));
            return -1;
    }

    // Set a timer to repeat every second
    timex_t interval = TIMEX_SECOND;
    timer_init(TIM_DEV(0), 1000000, timer_callback, NULL);

    while (1) {
        thread_sleep();  // Sleep to enable timer callback
    }

    // Close NVS storage handle
    nvs_close(nvs_handle);
    return 0;
}