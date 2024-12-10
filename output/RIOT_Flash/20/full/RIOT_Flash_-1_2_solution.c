#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <event/timeout.h>

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static void check_nvs_error(esp_err_t err) {
    if (err != ESP_OK) {
        printf("NVS error: %s\n", esp_err_to_name(err));
        xtimer_sleep(1);  // Sleep before retrying, if applicable
    }
}

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    while (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_device_state(char state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        printf("Error opening NVS handle!");
        check_nvs_error(err);
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    }

    err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    check_nvs_error(err);

    err = nvs_commit(my_handle);
    check_nvs_error(err);

    nvs_close(my_handle);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    init_nvs();
    
    char state = 1;  // Example initial state
    
    while (1) {
        // Alternate state between '1' and '0' for demonstration
        store_device_state(state);
        state = (state == 1) ? 0 : 1;
        
        // Wait for 1 second
        xtimer_periodic_wakeup(&last_wakeup, 1000000);
    }
    return 0;
}