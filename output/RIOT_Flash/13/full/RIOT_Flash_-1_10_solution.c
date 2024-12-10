#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>

// Function to check and retry on error
void check_error_and_retry(esp_err_t err, const char *action) {
    while (err != ESP_OK) {
        printf("Error %s: %s. Retrying...\n", action, esp_err_to_name(err));
        xtimer_sleep(1);
        err = action(); // Retry the action
    }
}

// Function to store device state
void store_device_state(nvs_handle_t my_handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, "device_state", state);
    check_error_and_retry(err, "nvs_set_u8"); // Check and retry if error occurs
    err = nvs_commit(my_handle);
    check_error_and_retry(err, "nvs_commit"); // Check and retry if error occurs
    printf("Device state %d stored successfully.\n", state);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    check_error_and_retry(err, "nvs_flash_init"); // Check and retry if error occurs

    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    check_error_and_retry(err, "nvs_open"); // Check and retry if error occurs

    uint8_t state = 0;

    while (1) {
        store_device_state(my_handle, state);
        state = !state; // Toggle state between 0 and 1
        xtimer_sleep(1); // Wait for one second before next update
    }

    nvs_close(my_handle);
    return 0;
}