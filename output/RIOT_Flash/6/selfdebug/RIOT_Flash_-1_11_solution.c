#include <ztimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"
#define TIMER_INTERVAL (1U * US_PER_SEC)

static ztimer_t timer;
static nvs_handle_t my_handle;

void timer_callback(void)
{
    // Toggle device state: assume you have a method to read current temperature.
    int device_state = read_device_state();

    // Save state to NVS
    esp_err_t err = nvs_set_i32(my_handle, "device_state", device_state);
    if (err != ESP_OK) {
        printf("Error (%s) saving state!\n", esp_err_to_name(err));
    }
    else {
        printf("Device state %d saved\n", device_state);
    }

    // Commit written value to NVS
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing state!\n", esp_err_to_name(err));
    }

    // Set the timer again
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);
}

int read_device_state(void)
{
    // Placeholder for reading actual device state
    static int state = 0;
    return state = !state;  // Toggle state for simulation purposes
}

int main(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    // Initialize ztimer and set callback
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    while (1) {
        // Let the timer do its work in callback for repeating every second
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
