#include <timer_arch.h>
#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>

#define STORAGE_NAMESPACE "storage"

static void write_state_to_nvs(int state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing!");
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit (%s)\n", esp_err_to_name(err));
    }
    nvs_close(my_handle);
}

static int read_state_from_nvs() {
    nvs_handle_t my_handle;
    int32_t state = 0;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }
    err = nvs_get_i32(my_handle, "device_state", &state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("The value is not initialized yet!\n");
    } else if (err != ESP_OK) {
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    nvs_close(my_handle);
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    int state = read_state_from_nvs();

    uint32_t last_wakeup = xtimer_now();

    while (1) {
        write_state_to_nvs(state);
        printf("Written state: %d\n", state);
        state = !state; // Toggle state

        xtimer_periodic_wakeup(&last_wakeup, 1000000);
    }
    return 0;
}