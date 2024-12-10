// Description: This code reads temperature from a sensor and stores its state ("1" for high, "0" for low) persistently in non-volatile storage (NVS) every second using the ESP32 and RIOT OS.

#include <ztimer.h>
#include <stdio.h>
#include <nvs.h>
#include <esp_system.h>
#include <dht.h>

#define SLEEP_DURATION (1U * US_PER_SEC)  // 1 second
#define TEMP_THRESHOLD 25                 // temperature threshold

static nvs_handle_t my_handle;
static char temp_state_key[] = "temp_state";

int main(void) {
    // Initialize the NVS
    if (nvs_open("storage", NVS_READWRITE, &my_handle) != ESP_OK) {
        puts("Error opening NVS handle");
        return 1;
    }

    // Sensor configuration
    dht_t dev;
    dht_params_t params;
    dht_init(&dev, &params);

    while (1) {
        // Get temperature
        int16_t temp;
        if (dht_read(&dev, &temp, NULL) != DHT_OK) {
            puts("Error reading temperature sensor");
        }

        // State assessment
        char state = (temp / 10) >= TEMP_THRESHOLD ? '1' : '0';

        // Write state to NVS
        if (nvs_set_str(my_handle, temp_state_key, &state) != ESP_OK) {
            puts("Error writing state to NVS");
        }

        nvs_commit(my_handle);

        ztimer_sleep(ZTIMER_MSEC, SLEEP_DURATION);
    }

    // Closing NVS handle (optional as while loop is continuous)
    nvs_close(my_handle);
    return 0;
}
