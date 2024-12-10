#include <xtimer.h>
#include <stdio.h>
#include <nvs.h>

#define DELAY        (1U * US_PER_SEC) // 1 second

static nvs_handle_t my_handle;
static const char* storage_key = "device_state";
static int device_state = 0;

static void init_nvs(void) {
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("NVS opened successfully\n");
    }
}

static void update_device_state(void) {
    // Write the new state to NVS
    esp_err_t err = nvs_set_i32(my_handle, storage_key, device_state);
    if (err != ESP_OK) {
        printf("Error (%s) setting value!\n", esp_err_to_name(err));
        return;
    }

    // Commit the change
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing value!\n", esp_err_to_name(err));
        return;
    }

    printf("Device state updated to: %d\n", device_state);

    // Toggle the state
    device_state = !device_state;
}

int main(void) {
    init_nvs();

    while (1) {
        update_device_state();
        xtimer_sleep(DELAY);
    }

    // Close NVS handle (not normally reached in this loop)
    nvs_close(my_handle);
    return 0;
}
