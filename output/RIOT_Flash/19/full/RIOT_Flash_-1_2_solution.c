#include <stdio.h>
#include <stdint.h>
#include <xtimer.h>
#include <nvs.h>
#include <esp_system.h>

#define STORAGE_NAMESPACE "storage"
#define KEY "state_key"
#define SLEEP_INTERVAL (1U)

static nvs_handle_t my_handle;

void check_nvs_op(esp_err_t result) {
    while (result != ESP_OK) {
        printf("NVS operation failed with error: %d. Retrying...\n", result);
        xtimer_sleep(SLEEP_INTERVAL);
        result = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    }
}

void initialize_nvs(void) {
    esp_err_t result = nvs_flash_init();
    check_nvs_op(result);

    result = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    check_nvs_op(result);
}

int32_t get_random_state() {
    return esp_random() % 2;
}

void toggle_device_state(void) {
    int32_t state = get_random_state();
    printf("Storing new state: %ld\n", state);
    esp_err_t result = nvs_set_i32(my_handle, KEY, state);
    check_nvs_op(result);
    result = nvs_commit(my_handle);
    check_nvs_op(result);
    nvs_close(my_handle);
}

int main(void) {
    initialize_nvs();
    while(1) {
        toggle_device_state();
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}