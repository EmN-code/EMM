#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <inttypes.h>
#include <nvs_flash.h>

#define STORAGE_NAMESPACE "storage"
#define STORAGE_KEY "device_state"

static nvs_handle_t my_handle;

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);
}

void store_device_state(int state) {
    esp_err_t err;
    err = nvs_set_i32(my_handle, STORAGE_KEY, state);
    ESP_ERROR_CHECK(err);
    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);
}

int read_device_state(void) {
    int32_t state = 0;
    esp_err_t err = nvs_get_i32(my_handle, STORAGE_KEY, &state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return -1; // or some default value
    }
    ESP_ERROR_CHECK(err);
    return state;
}


int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now(); // Correctly initialize xtimer

    printf("Initializing NVS...");
    init_nvs();

    while (1) {
        int state = read_device_state();
        printf("Current state: %d\n", state);

        // Toggle the device state
        state = (state == 0) ? 1 : 0;
        store_device_state(state);

        // Sleep for 1 second using correct function
        xtimer_periodic_wakeu...ge sleep function
    }
    nvs_close(my_handle); // Correctly close the NVS handle when done (though it's unreachable in this infinite loop)
    return 0;
}