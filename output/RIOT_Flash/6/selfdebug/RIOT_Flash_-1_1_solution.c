#include <stdio.h>
#include <stdint.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <periph/rtc.h>
#include <thread.h>

#define THREAD_PRIORITY       (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define NVS_NAMESPACE         "storage"

static char stack[THREAD_STACKSIZE_MAIN];

void *state_update_thread(void *arg) {
    nvs_handle_t nvs_handle;
    esp_err_t err;
    int8_t state = 0;

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return NULL;
    }

    while (1) {
        state = !state; // toggle state between 1 and 0
        err = nvs_set_i8(nvs_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error (%d) setting state!\n", err);
        }
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing state!\n", err);
        }
        printf("State updated to: %d\n", state);

        thread_sleep(); // sleep for 1 second
    }

    nvs_close(nvs_handle);
    return NULL;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS flash!\n", err);
        return err;
    }

    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY, 0,
                  state_update_thread, NULL, "state_update");

    return 0;
}