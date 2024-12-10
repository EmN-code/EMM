#include "xtimer.h"
#include "nvs.h"
#include "stdio.h"

#define INTERVAL (1U * 1000000U)  // 1 second interval in microseconds
#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static nvs_t nvs;

void setup_nvs(void) {
    int res = nvs_init(&nvs, NVS_NAMESPACE, NVS_SIZE);
    if (res < 0) {
        printf("Error initializing NVS: %d\n", res);
        return;
    }
    printf("NVS initialized successfully.\n");
}

void store_device_state(uint8_t state) {
    printf("Storing device state: %u\n", state);
    int res = nvs_set(&nvs, NVS_KEY, &state, sizeof(state));
    if (res < 0) {
        printf("Error storing device state: %d\n", res);
    } else {
        printf("Device state stored successfully.\n");
    }
}

uint8_t read_device_state(void) {
    uint8_t state;
    size_t len = sizeof(state);
    int res = nvs_get(&nvs, NVS_KEY, &state, &len);
    if (res < 0) {
        printf("Error reading device state: %d. Defaulting to 0.\n", res);
        return 0;
    }
    printf("Read device state: %u\n", state);
    return state;
}

int main(void) {
    setup_nvs();
    uint8_t state = read_device_state();
    while (1) {
        state = !state;  // Toggle state
        store_device_state(state);
        xtimer_sleep(1);
    }
    return 0;
}