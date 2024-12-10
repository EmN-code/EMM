#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"
#define UPDATE_INTERVAL 1

void update_device_state(nvs_handle my_handle) {
    static char state = '0';
    esp_err_t err;

    state = (state == '0') ? '1' : '0'; // Toggle state

    err = nvs_set_str(my_handle, "device_state", &state);
    if (err != ESP_OK) {
        printf("Error updating NVS (%s), retrying...\n", esp_err_to_name(err));
        return;
    }
    printf("Device state updated in NVS: %c\n", state);

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing NVS (%s), retrying...\n", esp_err_to_name(err));
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS (%s)\n", esp_err_to_name(err));
        return -1;
    }

    nvs_handle my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle (%s)\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        update_device_state(my_handle);
        xtimer_sleep(UPDATE_INTERVAL);
    }

    nvs_close(my_handle);
    return 0;
}