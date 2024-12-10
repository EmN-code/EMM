#include <ztimer.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <event/thread.h>
#include <netinet/in.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <mqtt_client.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <timex.h>
#include <thread.h>
#include <time_units.h>
#include <architecture.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mutex.h>

#define QUEUE_SIZE 8
#define INTERVAL (1000LU * US_PER_MS)

static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;
static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    while (1) {
        if (mbox_try_send(&mbox, &msg) != 0) {
            printf("Failed to send message. Retrying...\n");
            usleep(1000);
        } else {
            printf("Message sent successfully\n");
            break;
        }
    }
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Received message of type %04x\n", msg.type);
        }
    }
    return NULL;
}

int main(void) {
    ztimer_set(&timer, INTERVAL);
    ztimer_cb_set(&timer, timer_callback, NULL);
    ztimer_start(&timer);

    mbox_init(&mbox, queue, QUEUE_SIZE);

    thread_create(
        (char *)receiver_thread, sizeof(receiver_thread), 0, 0,
        (thread_task_func_t)receiver_thread, NULL, "receiver"
    );

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}