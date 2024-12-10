#include "nvs_handle.h"
#include "xtimer.h"
#include "stdio.h"
#include "thread.h"

// Use the correct RIOT headers and ensure nvs_handle.h is in the include path

#define STORAGE_NAMESPACE "storage"
#define DEFAULT_STATE 0
#define STATE_KEY "device_state"
#define TEMP_INTERVAL (1 * US_PER_SEC)

nvs_handle_t my_handle;

void store_state(int state) {
    esp_err_t err = nvs_set_i32(my_handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) storing state!\n", esp_err_to_name(err));
    }
    nvs_commit(my_handle);
}

int get_state(void) {
    int state = DEFAULT_STATE;
    esp_err_t err = nvs_get_i32(my_handle, STATE_KEY, &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        printf("Error (%s) reading state!\n", esp_err_to_name(err));
    }
    return state;
}

void update_storage(void) {
    int state = get_state();
    // Simulate toggling state between 0 and 1 for demonstration
    state = (state == 0) ? 1 : 0;
    printf("Toggling state to: %d\n", state);
    store_state(state);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK && err != ESP_ERR_NVS_NO_FREE_PAGES && err != ESP_ERR_NVS_NOT_INITIALIZED) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return -1;
    }
    
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        update_storage();
        xtimer_sleep(1);
    }
    
    nvs_close(my_handle);
    return 0;
}