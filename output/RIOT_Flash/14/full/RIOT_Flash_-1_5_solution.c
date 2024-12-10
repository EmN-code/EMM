#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "netinet/in.h"
#include "net/sock/util.h"

#define STORAGE_NAMESPACE "storage"

void initialize_nvs(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        nvs_flash_erase();
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
    }
}

void store_device_state(nvs_handle_t handle, const char* key, uint8_t value){
    esp_err_t err = nvs_set_u8(handle, key, value);
    if (err != ESP_OK) {
        printf("Error (%s) writing!\n", esp_err_to_name(err));
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing changes!\n", esp_err_to_name(err));
    }
}

uint8_t get_device_state(nvs_handle_t handle, const char* key){
    uint8_t value = 0;
    esp_err_t err = nvs_get_u8(handle, key, &value);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    return value;
}

int main(void){
    initialize_nvs();

    nvs_handle_t nvs_handle;
    nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);

    while(1) {
        uint8_t current_state = get_device_state(nvs_handle, "device_state");
        current_state = (current_state == 0) ? 1 : 0;
        printf("Storing state: %d\n", current_state);
        store_device_state(nvs_handle, "device_state", current_state);

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}