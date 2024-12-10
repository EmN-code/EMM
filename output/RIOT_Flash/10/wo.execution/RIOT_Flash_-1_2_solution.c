#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <event/thread.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"
#define KEY_DEVICE_STATE "device_state"
#define SLEEP_DURATION 1  // Sleep duration in seconds

void initialize_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return;

    // Write
    err = nvs_set_i32(my_handle, KEY_DEVICE_STATE, state);
    if (err != ESP_OK) {
        nvs_close(my_handle);
        return;
    }

    // Commit written value.
    nvs_commit(my_handle);

    // Close
    nvs_close(my_handle);
}

void *device_state_thread(void *arg) {
    int state = 0;
    while (1) {
        store_device_state(state);
        printf("Stored device state: %d\n", state);
        state = !state;  // Toggle state between 1 and 0
        sleep(SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    initialize_nvs();
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, device_state_thread, NULL, "device_state_thread");
    pause(); // Required to keep the main thread running
    return 0;
}
