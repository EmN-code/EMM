#include "xtimer.h"
#include "nvs.h"
#include "stdio.h"

#define INTERVAL (1000000) // 1 second in microseconds
define NVS_NAMESPACE "storage"

void store_device_state(nvs_t *nvs, int state) {
    if (nvs_set_int(nvs, NVS_NAMESPACE, "state", state) == NVS_OK) {
        printf("Stored value: %d\n", state);
    } else {
        printf("Error while storing the value\n");
    }
}

int main(void) {
    nvs_t nvs;
    int device_state = 0;

    if (nvs_init(&nvs, 