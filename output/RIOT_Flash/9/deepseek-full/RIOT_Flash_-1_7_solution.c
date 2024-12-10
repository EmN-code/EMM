
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <timex.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <nvs_flash.h>
#include <nvs.h>

#define QUEUE_SIZE 8
#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static nvs_handle_t nvs_handle;

void update_state(void) {
    static bool state = false;
    state = !state;

    esp_err_t err = nvs_set_u8(nvs_handle, NVS_KEY, state ? 1 : 0);
    if (err != ESP_OK) {
        printf("Failed to write to NVS (err %d)\n", err);
        return;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to commit to NVS (err %d)\n", err);
    }
}

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.ptr = NULL;
    msg_send(&msg, mbox_pid(&mbox));
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed (err %d)\n", err);
        return 1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle (err %d)\n", err);
        return 1;
    }

    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, 1 * US_PER_SEC);

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        update_state();
    }

    nvs_close(nvs_handle);
    return 0;
}
