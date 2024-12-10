#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "kernel.h"
#include "periph/timer.h"

#define NVS_NAMESPACE "storage"

static nvs_handle_t nvs_handle;
static int16_t temp = 25;  // Default temperature for illustration

void save_temp_to_nvs(int16_t temperature) {
    esp_err_t err;
    
    // Write temperature to the NVS
    err = nvs_set_i16(nvs_handle, "temperature", temperature);
    if (err != ESP_OK) {
        printf("Error writing to NVS:\n");
        return;
    }
    
    // Commit the written value
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Error committing to NVS:\n");
    }
}

void timer_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    
    // Simulate temperature change for demonstration
    temp = (temp == 25) ? 30 : 25;

    // Save updated temperature to NVS
    save_temp_to_nvs(temp);

    printf("Temperature saved: %d\n", temp);
}

int main(void) {
    puts("NVS example in RIOT OS");

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS:\n");
        return 1;
    }

    // Open NVS handle
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle:\n");
        return 1;
    }

    // Initialize a timer to trigger every second
    tim_t timer = TIMER_DEV(0);
    timer_init(timer, 1UL, timer_callback, NULL);
    timer_start(timer);

    return 0;
}