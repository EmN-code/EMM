#include <xtimer.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <esp_wifi.h>
#include <netinet/in.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <net/sock/util.h>
#include <msg.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <unistd.h>
#include <timex.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <evtimer_mbox.h>
#include <esp_system.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <mutex.h>
#include <net/gcoap.h>

#define STORAGE_NAMESPACE "storage"

void check_esp_err(esp_err_t err, const char *msg) {
    if (err != ESP_OK) {
        printf("Error (%s): %s\n", esp_err_to_name(err), msg);
    }
}

void initialize_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        err = nvs_flash_erase();
        check_esp_err(err, "Failed to erase NVS partition");
        err = nvs_flash_init();
        check_esp_err(err, "Failed to initialize NVS");
    }
}

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return;
    }

    err = nvs_set_i32(my_handle, "device_state", state);
    check_esp_err(err, "Failed to set device state in NVS");

    err = nvs_commit(my_handle);
    check_esp_err(err, "Failed to commit changes in NVS");

    nvs_close(my_handle);
}

int main(void) {
    initialize_nvs();

    int state = 0;

    while (1) {
        store_device_state(state);
        state = !state;
        xtimer_sleep(1);
    }

    return 0;
}
