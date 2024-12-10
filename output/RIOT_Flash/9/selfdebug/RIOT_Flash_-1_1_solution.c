#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include <thread.h>
#include <ztimer.h>

#define STORAGE_NAMESPACE "storage"

static kernel_pid_t main_pid;
static char stack[THREAD_STACKSIZE_MAIN];

void write_state_to_nvs(int32_t state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
    }

    nvs_close(my_handle);
}

void *state_updater_thread(void *arg) {
    (void)arg;
    int32_t state = 0;

    while (1) {
        write_state_to_nvs(state);
        printf("State updated: %d\n", state);
        state = !state; // Toggle state between 0 and 1
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    main_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                             THREAD_CREATE_STACKTEST, state_updater_thread,
                             NULL, "state_updater");

    return 0;
}