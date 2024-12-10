#include <stdio.h>
#include <nvs_flash.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define DELAY_S (1) // 1 second delay

nvs_handle_t my_handle;

void init_flash(void);
void toggle_device_state(void);

void init_flash(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void toggle_device_state(void) {
    int32_t state = 0;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    } else {
        printf("Done\n");

        // Read
        err = nvs_get_i32(my_handle, STATE_KEY, &state);
        switch (err) {
            case ESP_OK:
                printf("Done\n");
                printf("State = %d\n", state);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                state = 0;
                break;
            default:
                printf("Error (%s) reading!\n", esp_err_to_name(err));
                break;
        }

        // Toggle and Write
        state = !state;
        err = nvs_set_i32(my_handle, STATE_KEY, state);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }
}

int main(void) {
    init_flash();
    while (1) {
        toggle_device_state();
        xtimer_sleep(DELAY_S);
    }
    return 0;
}