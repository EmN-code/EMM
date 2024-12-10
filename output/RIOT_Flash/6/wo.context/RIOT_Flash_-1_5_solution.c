#include <xtimer.h>
#include <nvs.h>
#include <stdio.h>

static nvs_handle_t nvs_handle;

void store_device_state(const int state) {
    nvs_set_i32(nvs_handle, "device_state", state);
    nvs_commit(nvs_handle);
}

int main(void) {
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    int state = 0;
    while (1) {
        store_device_state(state);
        printf("Stored device state: %d\n", state);
        state = !state;  // Toggle state between 0 and 1
        xtimer_sleep(1); // Wait for 1 second
    }

    nvs_close(nvs_handle);
    return 0;
}