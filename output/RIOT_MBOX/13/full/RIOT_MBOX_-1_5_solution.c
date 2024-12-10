#include <xtimer.h>
#include <ztimer.h>
#include <ztimer_sleep.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <arp\a/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <mbox.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define MSG_QUEUE_SIZE (8)
#define INTERVAL (5000000) // 5 seconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_DEFAULT];

void* thread_handler(void *arg) {
    msg_t msg;
    int counter = 0;
    while (1) {
        msg_receive(&msg);
        counter++;
        printf("Received message: %d\n", counter);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                    THREAD_CREATE_STACKTEST, thread_handler,
                                    NULL, "message_thread");

    if (pid < 0) {
        printf("[Error] Failed to create thread\n");
        return 1;
    }

    msg_t msg;
    while (1) {
        msg_send_receive(&msg, NULL, pid);
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}