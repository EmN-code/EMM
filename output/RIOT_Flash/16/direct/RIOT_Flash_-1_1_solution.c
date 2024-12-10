#include <stdio.h>
#include <stdint.h>
#include "thread.h"
#include "xtimer.h"
#include "nvs.h"

#define NVS_STORAGE "nvs"
#define STATE_KEY "state"
#define INTERVAL (1U * US_PER_SEC) // 1 second interval

void store_state_in_nvs(uint8_t state) {
    nvs_t nvs;
    if (nvs_init(&nvs, NVS_STORAGE) != NVS_OK) {
        printf("Error initializing NVS\n");
        return;
    }

    if (nvs_set(&nvs, STATE_KEY, &state, sizeof(state), 0) != NVS_OK) {
        printf("Error storing state in NVS\n");
        nvs_close(&nvs);
        return;
    }
    printf("State %d stored in NVS\n", state);
    nvs_close(&nvs);
}

uint8_t read_state_from_nvs(void) {
    nvs_t nvs;
    uint8_t state = 0;

    if (nvs_init(&nvs, NVS_STORAGE) != NVS_OK) {
        printf("Error initializing NVS\n");
        return state;
    }

    if (nvs_get(&nvs, STATE_KEY, &state, sizeof(state), 0) != NVS_OK) {
        printf("Error reading state from NVS\n");
    }
    else {
        printf("State read from NVS: %d\n", state);
    }
    nvs_close(&nvs);
    return state;
}

void *state_handler(void *arg) {
    (void)arg;
    uint8_t state = read_state_from_nvs();

    while (1) {
        state = !state; // Toggle state
        store_state_in_nvs(state);
        xtimer_sleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    printf("Starting NVS state storage program\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, state_handler, NULL, "state_handler");

    return 0;
}