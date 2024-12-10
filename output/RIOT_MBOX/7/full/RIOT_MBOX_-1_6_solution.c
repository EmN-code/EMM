#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/util.h>
#include <string.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <stddef.h>
#include <ztimer.h>
#include <kernel_defines.h>
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
#include <dht.h>
#include <sys/socket.h>
#include <net/gcoap.h>

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static kernel_pid_t msg_thread_pid;
static char msg_thread_stack[THREAD_STACKSIZE_MAIN];

static void *_msg_thread(void *arg) {
    (void)arg;
    msg_t msg;
    mbox_t msg_box;
    msg_t msg_q[8];

    mbox_init(&msg_box, msg_q, 8);

    while (1) {
        if (mbox_get(&msg_box, &msg, true) == 1) {
            printf("Message received: %u\n", msg.content.value);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    printf("Message box example on ESP32 starting.\n");

    msg_thread_pid = thread_create(msg_thread_stack, sizeof(msg_thread_stack), THREAD_PRIORITY_MAIN - 1,
                                   THREAD_CREATE_STACKTEST, _msg_thread, NULL, "msg_thread");

    mbox_t msg_box;
    msg_t msg_q[8];
    mbox_init(&msg_box, msg_q, 8);

    while (1) {
        msg_t msg;
        msg.content.value = (uint32_t)xtimer_now_usec();
        mbox_put(&msg_box, &msg, true);
        printf("Message sent: %u\n", msg.content.value);
        xtimer_sleep(5);
    }

    return 0;
}