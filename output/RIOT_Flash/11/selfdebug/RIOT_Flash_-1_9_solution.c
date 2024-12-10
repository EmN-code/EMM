#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

static nvs_handle_t my_handle;

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_device_state(int32_t state) {
    esp_err_t err;
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) setting NVS state\n", esp_err_to_name(err));
    } else {
        nvs_commit(my_handle);
        printf("Stored value: %ld\n", (long)state);  // Changed %d to %ld for int32_t compatibility
    }
}

int32_t get_device_state(void) {
    int32_t state = 0;
    esp_err_t err;
    err = nvs_get_i32(my_handle, "device_state", &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        printf("Error (%s) reading NVS state\n", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    init_nvs();

    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        int32_t state = get_device_state();
        state = (state == 1) ? 0 : 1;  // Toggle state

        store_device_state(state);

        xtimer_sleep(1);
    }

    // Close NVS handle before exiting (never reached in this example)
    // nvs_close(my_handle);
    return 0;
}