#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#ifndef THREAD_STACKSIZE_MAIN
#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT
#endif

#define STATE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define TOGGLE_INTERVAL (1U * US_PER_SEC)

static int toggle_device_state(void) {
    /* Initialize NVS */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STATE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    /* Try to read previous state */
    int32_t state = 0;
    err = nvs_get_i32(my_handle, STATE_KEY, &state);
    switch (err) {
        case ESP_OK:
            printf("Device state = %d\n", state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The state is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading state from NVS!\n", esp_err_to_name(err));
            return err;
    }

    /* Toggle state */
    state = !state;

    /* Write updated value back to NVS */
    printf("Updating state in NVS to %d\n", state);
    err = nvs_set_i32(my_handle, STATE_KEY, state);
    ESP_ERROR_CHECK(err);

    /* Commit updates to NVS */
    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);

    /* Close NVS handle */
    nvs_close(my_handle);

    return ESP_OK;
}

int main(void) {
    puts("Starting device state toggle application...");

    while (1) {
        toggle_device_state();
        xtimer_sleep(TOGGLE_INTERVAL);
    }

    return 0;
}