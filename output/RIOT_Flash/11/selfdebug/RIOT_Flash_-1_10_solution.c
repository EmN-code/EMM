#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"

void store_state(nvs_handle_t my_handle, int state) {
    nvs_set_i32(my_handle, "device_state", state);
    nvs_commit(my_handle);
}

int retrieve_state(nvs_handle_t my_handle) {
    int state = 0;
    nvs_get_i32(my_handle, "device_state", &state);
    return state;
}

void initialize_nvs(nvs_handle_t *my_handle) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
    nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, my_handle);
}

void update_temperature(nvs_handle_t my_handle) {
    while(1) {
        int temperature = random_uint32_range(0, 50); // Simulates temperature fetching
        nvs_set_i32(my_handle, "temperature", temperature);
        nvs_commit(my_handle);
        printf("Stored temperature: %d\n", temperature);
        xtimer_sleep(1);
    }
}

int main(void) {
    nvs_handle_t my_handle;
    initialize_nvs(&my_handle);

    int current_state = retrieve_state(my_handle);
    printf("Retrieved state: %d\n", current_state);

    store_state(my_handle, current_state == 0 ? 1 : 0);

    update_temperature(my_handle);

    return 0;
}