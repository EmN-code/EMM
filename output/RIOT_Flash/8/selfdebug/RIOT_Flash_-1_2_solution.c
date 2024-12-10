#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include "thread.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define THREAD_STACKSIZE_MAIN (1024 * 4)

static char _stack[THREAD_STACKSIZE_MAIN];

void save_device_state(int state) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    err = nvs_set_i32(handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to set state (%d)!\n", err);
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Failed to commit state (%d)!\n", err);
    }

    nvs_close(handle);
}

void *thread_function(void *arg) {
    (void)arg;
    int state = 0;
    while (1) {
        save_device_state(state);
        state = (state == 0) ? 1 : 0;
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    thread_create(_stack, sizeof(_stack), THREAD_PRIORITY_MAIN - 1, 0, thread_function, NULL, "state_thread");

    return 0;
}