#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <esp_system.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <time.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"

static void update_device_state(nvs_handle_t handle) {
    int32_t state = 0;
    esp_err_t err = nvs_get_i32(handle, "state", &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        printf("Error (%s) reading!
", esp_err_to_name(err));
    }

    state = !state;

    err = nvs_set_i32(handle, "state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing!
", esp_err_to_name(err));
    }

    printf("Device state updated to %ld
", state);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!
", esp_err_to_name(err));
        return err;
    }

    while (1) {
        update_device_state(my_handle);
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}