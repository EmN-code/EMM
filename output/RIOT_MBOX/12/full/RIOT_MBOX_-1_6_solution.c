#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <timex.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <dht.h>
#include <dht_params.h>
#include <ztimer.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define THREAD_STACKSIZE_MAIN (1024)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

msg_t msg_queue[8];

static char thread_stack[THREAD_STACKSIZE_MAIN];

static void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;

    while (1) {
        msg_send_receive(&msg, &reply, thread_getpid());
        printf("Received message with type: %u\n", msg.type);
        xtimer_sleep(1);
    }
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    kernel_pid_t pid = thread_create(thread_stack, sizeof(thread_stack),
                                     THREAD_PRIORITY_MAIN, 0,
                                     sender_thread, NULL, "sender");

    msg_t msg;
    unsigned int counter = 0;

    while (1) {
        msg.type = counter++;
        msg_send_receive(&msg, NULL, pid);
        xtimer_sleep(5);
    }

    return 0;
}