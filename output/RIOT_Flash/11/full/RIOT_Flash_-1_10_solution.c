#include "stdatomic.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"
#include "unistd.h"
#include "ztimer.h"
#include "msg.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_system.h"

#define SLEEP_DURATION (1000000U)

static void initialize_nvs(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }
    if (ret != ESP_OK) {
        fprintf(stderr, "NVS Flash init failed: %s\n", esp_err_to_name(ret));
        exit(EXIT_FAILURE);
    }
}

static void store_device_state(nvs_handle_t handle, const char *key, int32_t value) {
    esp_err_t ret = nvs_set_i32(handle, key, value);
    if (ret != ESP_OK) {
        fprintf(stderr, "Error (%s) setting!");
        return;
    }
    ret = nvs_commit(handle);
    if (ret != ESP_OK) {
        fprintf(stderr, "Error (%s) committing!");
    }
}

int main(void) {
    initialize_nvs();

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        fprintf(stderr, "Error (%s) opening NVS handle!");
        return EXIT_FAILURE;
    }

    int32_t state = 0;
    while (1) {
        state = (state == 0) ? 1 : 0;
        printf("Storing state: %d\n", state);
        store_device_state(nvs_handle, "device_state", state);
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    nvs_close(nvs_handle);
    return EXIT_SUCCESS;
}