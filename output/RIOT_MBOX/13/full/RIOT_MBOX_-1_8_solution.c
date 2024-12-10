#include <ztimer.h>
#include <xtimer.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <msg.h>
#include <MQTTClient.h>
#include <esp_system.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <dht.h>
#include "MBQTTClient.h"  // This is the corrected header include

#define MSG_QUEUE_SIZE   (8U)
#define SENDER_THREAD    (1U)

static char sender_thread_stack[THREAD_STACKSIZE_MAIN];

static void *sender_function(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t queue[MSG_QUEUE_SIZE];

    msg_init_queue(queue, MSG_QUEUE_SIZE);
    while (1) {
        msg.content.value = 1;
        int res = msg_try_send(&msg, receiver_thread_pid);
        if (res == 0) {
            puts("[Error]: Message send failed.");
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_MAIN - SENDER_THREAD,
                  THREAD_CREATE_STACKTEST,
                  sender_function,
                  NULL, "sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return 0;
}