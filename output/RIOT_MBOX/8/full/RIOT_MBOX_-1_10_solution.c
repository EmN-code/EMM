#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/util.h>
#include <string.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <esp_wifi.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <mutex.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <arpa/inet.h>
#include <timex.h>
#include <esp_system.h>
#include <dht.h>
#include <sys/socket.h>
#include <net/gcoap.h>

#define MSG_TYPE_EVENT (0x6101)
#define EVENT_INTERVAL (1000000U)

static msg_t msg_queue[8];
static mbox_t mbox;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = MSG_TYPE_EVENT;

    while (1) {
        xtimer_usleep(EVENT_INTERVAL);
        msg_send(&msg, thread_getpid());
        mbox_put(&mbox, &msg, false);
        printf("Event sent\n");
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Event received\n");
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    mbox_init(&mbox, msg_queue, 8);

    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, 0,
                  sender_thread, NULL, "sender");
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 2, 0,
                  receiver_thread, NULL, "receiver");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }
    return 0;
}
