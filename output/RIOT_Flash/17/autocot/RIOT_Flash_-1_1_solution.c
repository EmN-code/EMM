#include "thread.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>
#include "xtimer.h"

#define NVS_NAMESPACE "storage"
#define STATE_KEY "dev_state"
#define DELAY 1000000 // 1 second in microseconds

void nvs_store_state(int state) {
    nvs_handle_t nvs_handle;
    esp_err_t err;

    /* Initialize NVS */
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    /* Open NVS handle */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    /* Write state to NVS */
    err = nvs_set_i32(nvs_handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to write to NVS\n");
    }

    /* Commit the written value */
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to commit to NVS\n");
    }

    /* Close NVS handle */
    nvs_close(nvs_handle);
}

void *temperature_update_thread(void *arg) {
    int state = 0;

    while (1) {
        nvs_store_state(state);
        // Toggle state
        state = !state;
        // Wait for a second
        xtimer_usleep(DELAY);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, temperature_update_thread,
                  NULL, "temperature_update");

    // Let the system idle
    while(1) {
        thread_sleep();
    }
    return 0;
}
