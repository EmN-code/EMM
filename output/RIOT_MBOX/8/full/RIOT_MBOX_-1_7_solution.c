#include <ztimer.h>
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
#include <stddef.h>
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
#include <net/gcoap.h>

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t main_thread;

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        xtimer_sleep(2);
        msg.type = 0x1234;
        msg_send(&msg, main_thread);
        printf("[Thread] Sent message with type: %u\n", msg.type);
    }
    return NULL;
}

int main(void) {
    puts("RIOT Message Box Example");

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    main_thread = thread_getpid();

    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread");

    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("[Main] Received message with type: %u\n", msg.type);
        }
    }

    return 0;
}