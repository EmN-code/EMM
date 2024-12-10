#include <xtimer.h>
#include <stdio.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_err.h>

#define STORAGE_NAMESPACE "storage"

void store_device_state(nvs_handle_t handle, int state) {
    esp_err_t err;
    
    err = nvs_set_i32(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Failed to write to NVS!\n");
    }
    
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Failed to commit changes to NVS!\n");
    }
}

int retrieve_device_state(nvs_handle_t handle) {
    int state = 0;
    esp_err_t err;

    err = nvs_get_i32(handle, "device_state", &state);
    switch (err) {
        case ESP_OK:
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        int state = retrieve_device_state(my_handle);
        printf("Current device state: %d\n", state);

        state = (state == 0) ? 1 : 0;  // toggle state
        store_device_state(my_handle, state);

        xtimer_sleep(1);  // wait for 1 second before next update
    }

    nvs_close(my_handle);
    return 0;
}