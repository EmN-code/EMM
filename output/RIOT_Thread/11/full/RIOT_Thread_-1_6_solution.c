#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <unistd.h>
#include "die.h"
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <kernel_defines.h>
#include <architecture.h>
#include <time_units.h>
#include <mutex.h>

#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_MAIN (1 << 10)  // A custom macro for stack size

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

msg_t queue[8];  // Define a message queue with a size of 8

void *thread_a_handler(void *arg) {
    (void) arg;
    msg_t msg;
    msg.content.value = (uint32_t) "Hello from A!";

    while (1) {
        msg_send(&msg, thread_getpid());  // Send messages indefinitely
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }
    return NULL;
}

void *thread_b_handler(void *arg) {
    (void) arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received: %s\n", (char*) msg.content.ptr);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    kernel_pid_t thread_a_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_a_handler, NULL, "thread_a");
    kernel_pid_t thread_b_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_b_handler, NULL, "thread_b");

    if (thread_a_pid <= KERNEL_PID_UNDEF || thread_b_pid <= KERNEL_PID_UNDEF) {
        printf("Error in creating threads\n");
        return 1;
    }
    return 0;
}
