#include <string.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <timex.h>
#include <ztimer.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY_STATE "device_state"
#define INTERVAL (1U * US_PER_SEC)  // 1 second in microseconds

// Function to initialize NVS
static void nvs_init(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        fprintf(stderr, "Error (%s) initializing NVS!\n", esp_err_to_name(err));
    }
}

// Function to store the state in NVS
static void store_state(int state) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        fprintf(stderr, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    err = nvs_set_i32(handle, NVS_KEY_STATE, state);
    if (err != ESP_OK) {
        fprintf(stderr, "Error (%s) setting int value!\n", esp_err_to_name(err));
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        fprintf(stderr, "Error (%s) committing value!\n", esp_err_to_name(err));
    }

    nvs_close(handle);
}

// Function to retrieve the state from NVS
static int retrieve_state(void) {
    nvs_handle_t handle;
    int32_t state = 0;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err == ESP_OK) {
        err = nvs_get_i32(handle, NVS_KEY_STATE, &state);
        if (err != ESP_OK) {
            fprintf(stderr, "Error (%s) getting int value!\n", esp_err_to_name(err));
        }
        nvs_close(handle);
    } else {
        fprintf(stderr, "Error (%s) opening NVS handle for reading!\n", esp_err_to_name(err));
    }
    return state;
}

// Timer callback
static void timer_callback(void *arg) {
    (void)arg;
    int current_state = retrieve_state();
    // Simulate temperature check; here we toggle between 0 and 1
    int new_state = (current_state == 0) ? 1 : 0;
    store_state(new_state);
    printf("Stored new state: %d\n", new_state);
}

int main(void) {
    nvs_init();

    // Start a timer that calls the callback every second
    ztimer_t timer;
    ztimer_set_msg(&timer, ZTIMER_ID_LOPOWER, INTERVAL, timer_callback, NULL);

    // Main event loop
    while (1) {
        // We rely on the timer to trigger updates
        ztimer_sleep(ZTIMER_ID_LOPOWER, INTERVAL);
    }
    return 0;
}