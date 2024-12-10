#include <xtimer.h>  // for xtimer_sleep
#include <stdlib.h>  // for atoi
#include <string.h>  // manipulating strings
#include <nvs_flash.h>  // for NVS operations
#include <nvs.h>  // for handling NVS tables
#include <mutex.h>  // for synchronization
#include <msg.h>  // for message queues
#include <timex.h>  // time-related functions
#include <thread.h>  // for thread handling

#define STACKSIZE_THREAD (THREAD_STACKSIZE_MAIN - 512)

char stack[STACKSIZE_THREAD];

nvs_handle_t my_handle;

// Thread function to update device state periodically
void *toggle_device_state(void *arg) {
    (void)arg;
    int state = 0;
    
    while (1) {
        state = !state;  // Toggle state
        nvs_set_i32(my_handle, "device_state", state);
        nvs_commit(my_handle);
        xtimer_sleep(1);  // Wait for 1 second
    }
    return NULL;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Create thread to toggle device state
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, 
                  toggle_device_state, NULL, "device_state_toggle");

    return 0;
}