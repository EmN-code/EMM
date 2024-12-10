#include "thread.h"
#include "stdio.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!
", err);
        return;
    }

    // Write
    err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%d) writing!
", err);
    }

    // Commit written value.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) committing!
", err);
    }

    // Close
    nvs_close(my_handle);
}

static void *device_state_update(void *arg) {
    (void)arg;
    int device_state = 0;

    while (1) {
        // Toggle device state
        device_state = !device_state;
        
        // Store device state in NVS
        store_device_state(device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
                  device_state_update, NULL, "device_state_update");

    return 0;
}
