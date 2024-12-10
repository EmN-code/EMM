#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "msg.h"
#include "nvs.h"

#define STATE_KEY "device_state"
#define NVS_NAMESPACE "storage"

// Define stack size and priority for the thread
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

char stack[THREAD_STACKSIZE];

void *state_persisting_thread(void *arg) {
    (void)arg;
    nvs_t nvs;
    char current_state[2] = "0"; // Initial state as "0"

    // Open NVS with the defined namespace
    if (nvs_open(&nvs, NVS_NAMESPACE, NVS_READWRITE) != NVS_OK) {
        printf("Error: unable to open NVS namespace\n");
        return NULL;
    }

    while (1) {
        // Simulating state change
        if (current_state[0] == '0') {
            current_state[0] = '1';
        } else {
            current_state[0] = '0';
        }

        // Write state to NVS
        if (nvs_set(&nvs, STATE_KEY, (void *)current_state, sizeof(current_state)) != NVS_OK) {
            printf("Error: unable to write to NVS\n");
        } else {
            printf("State %s written to NVS\n", current_state);
        }

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    nvs_close(&nvs);
    return NULL;
}

int main(void) {
    printf("Starting ESP32 device state persisting application\n");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  state_persisting_thread, NULL, "state_persisting_thread");

    return 0;
}
