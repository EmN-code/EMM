
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, const char *state) {
    nvs_set_str(handle, NVS_KEY, state);
    nvs_commit(handle);
}

void read_state(nvs_handle_t handle) {
    char state[2];
    size_t required_size;
    nvs_get_str(handle, NVS_KEY, NULL, &required_size);
    nvs_get_str(handle, NVS_KEY, state, &required_size);
    printf("Current state: %s\n", state);
}

int main(void) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    while (1) {
        // Simulate temperature update and store state
        const char *state = (rand() % 2 == 0) ? "1" : "0";
        store_state(handle, state);
        read_state(handle);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    nvs_close(handle);
    return 0;
}
