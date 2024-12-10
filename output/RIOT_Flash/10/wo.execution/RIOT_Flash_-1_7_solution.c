#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define STORAGE_NAMESPACE "storage"

void setup_nvs(nvs_handle_t *my_handle) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, my_handle));
}

void store_device_state(nvs_handle_t my_handle) {
    int32_t device_state = 0;
    ESP_ERROR_CHECK(nvs_set_i32(my_handle, "device_state", device_state));
    ESP_ERROR_CHECK(nvs_commit(my_handle));
}

void toggle_device_state() {
    nvs_handle_t my_handle;
    setup_nvs(&my_handle);

    while (1) {
        store_device_state(my_handle);

        xtimer_sleep(1);
    }

    nvs_close(my_handle);
}

// Main function to initiate the toggle feature
int main(void) {
    toggle_device_state();
    return 0;
}
