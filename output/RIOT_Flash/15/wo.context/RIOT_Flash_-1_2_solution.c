#include <xtimer.h>
#include <nvs_flash.h>

#define STATE_KEY "device_state"
#define STORAGE_NAMESPACE "storage"

void set_device_state(nvs_handle_t handle, int32_t state) {
    esp_err_t err = nvs_set_i32(handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to set state in NVS!\n");
    } else {
        printf("State set to %ld\n", state);
    }
    nvs_commit(handle);
}

void update_device_state(nvs_handle_t handle) {
    int32_t state = 1; // Example state
    set_device_state(handle, state);
    xtimer_sleep(1);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash initialization failed!\n");
        return -1;
    }
    
    nvs_handle_t handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle!\n");
        return -1;
    }

    while (1) {
        update_device_state(handle);
    }

    nvs_close(handle);
    return 0;
}