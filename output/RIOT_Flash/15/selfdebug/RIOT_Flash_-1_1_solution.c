#include "timex.h"
#include "ztimer.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

int read_device_state(void) {
    nvs_handle_t my_handle;
    int32_t state = 0;
    
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        err = nvs_get_i32(my_handle, STATE_KEY, &state);
        if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
            printf("Error reading device state from NVS\n");
        }
        nvs_close(my_handle);
    } else {
        printf("Error opening NVS handle\n");
    }
    return state;
}

void write_device_state(int32_t state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        err = nvs_set_i32(my_handle, STATE_KEY, state);
        if (err != ESP_OK) {
            printf("Error writing device state to NVS\n");
        } else {
            nvs_commit(my_handle);
        }
        nvs_close(my_handle);
    } else {
        printf("Error opening NVS handle for writing\n");
    }
}

void toggle_state_and_store(void) {
    static int32_t state = -1;
    if (state < 0) { // Uninitialized
        state = read_device_state();
    }
    state = !state; // Toggle state
    printf("New device state: %d\n", state);
    write_device_state(state);
}

void *toggle_thread(void *arg) {
    (void)arg;
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
        toggle_state_and_store();
    }
    return NULL;
}

int main(void) {
    init_nvs();
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, toggle_thread, NULL, "toggle_thread");
    return 0;
}