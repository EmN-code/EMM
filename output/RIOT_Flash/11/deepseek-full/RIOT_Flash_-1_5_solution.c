
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <ztimer.h>
#include <nvs.h>
#include <nvs_flash.h>

#define QUEUE_SIZE 8
#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t et;

void update_state(void) {
    static bool state = false;
    state = !state;

    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    err = nvs_set_u8(handle, NVS_KEY, state ? 1 : 0);
    if (err != ESP_OK) {
        printf("Error (%s) setting NVS value!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing NVS value!\n", esp_err_to_name(err));
        }
    }

    nvs_close(handle);
}

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.ptr = NULL;
    msg_send(&msg, &mbox);
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    evtimer_init_msg(&et);
    evtimer_add_msg(&et, (xtimer_ticks32_t)xtimer_ticks_from_usec(1000000), timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&mbox, &msg);
        update_state();
    }

    return 0;
}
