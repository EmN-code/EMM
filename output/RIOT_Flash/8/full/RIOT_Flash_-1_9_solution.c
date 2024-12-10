#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"

void handle_error(esp_err_t err) {
    if (err != ESP_OK) {
        printf("Error: %s\n", esp_err_to_name(err));
    }
}

int main() {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    handle_error(err);

    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    handle_error(err);

    int32_t temperature = 0;

    while (true) {
        // Simulate reading a temperature sensor value
        temperature += 1;

        // Store the temperature in NVS
        err = nvs_set_i32(my_handle, "temperature", temperature);
        handle_error(err);

        // Commit written value to NVS
        err = nvs_commit(my_handle);
        handle_error(err);

        printf("Temperature has been updated to %ld\n", temperature);

        // Wait for one second
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}