#include <ztimer.h>      // Ztimer for sleep function
#include <xtimer.h>      // For xtimer_sleep
#include <time_units.h>  // Time unit conversion
#include <thread.h>      // Thread and sleep utilities
#include <nvs_flash.h>   // NVS flash initialization
#include <nvs.h>         // NVS operation functions
#include <evtimer_msg.h> // Msg handler for event timer
#include <evtimer_mbox.h>// Mbox handler for event timer
#include <event/thread.h>// Event handling through threads
#include <timex.h>       // For time conversion

#define STORAGE_NAMESPACE "storage"

static nvs_handle_t my_handle;

void update_device_state(int state) {
    esp_err_t err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error setting device state: %d\n", err);
    } else {
        nvs_commit(my_handle);
        printf("Device state updated to %d\n", state);
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return err;
    }

    int state = 0;
    while (1) {
        update_device_state(state);
        state = !state; // Toggle state
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }

    nvs_close(my_handle);
    return 0;
}