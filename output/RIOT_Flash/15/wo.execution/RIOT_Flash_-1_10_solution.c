#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Placeholders for NVS functions
// In actual implementation, real NVS functions would be used
void nvs_open(const char *name, int mode, int *handle) {
    // Simulated opening of NVS
    *handle = 1;
}

void nvs_set_i32(int handle, const char *key, int value) {
    // Simulated setting of an integer value in NVS
    printf("Set %s = %d in NVS (handle: %d)\n", key, value, handle);
}

void nvs_commit(int handle) {
    // Simulated commit of changes to NVS
    printf("NVS changes committed (handle: %d)\n", handle);
}

void nvs_close(int handle) {
    // Simulated closing of NVS handle
    printf("NVS handle %d closed\n", handle);
}

void *update_nvs_state(void *arg) {
    int my_handle;
    int temperature = 0;
    nvs_open("storage", 1, &my_handle); // pseudo-mode for NVS_READWRITE

    while (1) {
        // Simulating a temperature reading
        temperature = (temperature + 1) % 100;  // just an example
        nvs_set_i32(my_handle, "temperature", temperature);
        nvs_commit(my_handle);
        
        ztimer_sleep(ZTIMER_SEC, 1);  // Sleep for 1 second
    }

    nvs_close(my_handle);

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  update_nvs_state, NULL, "update_nvs");

    while(1) {
        // Main thread could perform other tasks here, or just sleep
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}