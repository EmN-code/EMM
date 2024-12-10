// Include necessary headers
#include <xtimer.h>
#include <stdio.h>
#include <nvs.h>
#include <nvs_flash.h>

static nvs_handle_t my_handle;

static void check_nvs_init(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
}

static void store_state(uint32_t state) {
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        nvs_set_i32(my_handle, "device_state", state);
        nvs_commit(my_handle);
        nvs_close(my_handle);
    }
}

static uint32_t get_state(void) {
    uint32_t state = 0;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &my_handle);
    if (err == ESP_OK) {
        nvs_get_i32(my_handle, "device_state", (int32_t*)&state);
        nvs_close(my_handle);
    }
    return state;
}

int main(void) {
    check_nvs_init();
    while (1) {
        uint32_t state = get_state();
        printf("Current device state: %u\n", state);
        state = !state;  // Toggle state
        store_state(state);
        xtimer_sleep(1);  // Wait for 1 second
    }
    return 0;
}