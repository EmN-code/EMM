#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <nvs.h>

#define DELAY (1U * US_PER_SEC)
#define NVS_NAMESPACE "storage"
#define STORAGE_KEY "device_state"

void handle_nvs_error(esp_err_t err) {
    if (err != ESP_OK) {
        printf("NVS interaction failed: %s\n", esp_err_to_name(err));
    }
}

void store_device_state(nvs_handle_t nvs_handle, int state) {
    esp_err_t err = nvs_set_i32(nvs_handle, STORAGE_KEY, state);
    handle_nvs_error(err);
    
    err = nvs_commit(nvs_handle);
    handle_nvs_error(err);
}

void init_nvs(nvs_handle_t *nvs_handle) {
    esp_err_t err = nvs_flash_init();
    handle_nvs_error(err);

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, nvs_handle);
    handle_nvs_error(err);
}

int read_device_state(nvs_handle_t nvs_handle) {
    int32_t state = 0;
    esp_err_t err = nvs_get_i32(nvs_handle, STORAGE_KEY, &state);
    handle_nvs_error(err);
    return state;
}

int main(void) {
    nvs_handle_t nvs_handle;
    init_nvs(&nvs_handle);

    while (1) {
        int current_state = read_device_state(nvs_handle);
        printf("Current device state: %d\n", current_state);

        int new_state = (current_state == 0) ? 1 : 0;
        store_device_state(nvs_handle, new_state);
        printf("New device state stored: %d\n", new_state);

        xtimer_sleep(1);
    }

    return 0;
}