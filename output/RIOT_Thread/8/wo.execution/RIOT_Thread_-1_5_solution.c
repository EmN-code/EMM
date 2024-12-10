#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <msg.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <mbox.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

msg_t queue_one[8];
msg_t queue_two[8];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void) arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 100;
    msg_send(&msg, thread_two_pid);
    return NULL;
}

void *thread_two(void *arg) {
    (void) arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Received message with type: %d and value: %lu\n", msg.type, (unsigned long)msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(queue_one, ARRAY_SIZE(queue_one));
    msg_init_queue(queue_two, ARRAY_SIZE(queue_two));

    thread_one_pid = thread_create(stack_one, THREAD_STACKSIZE_MAIN,
                                   THREAD_PRIORITY_MAIN - 1, CREATE_STACKTEST,
                                   thread_one, NULL, "thread_one");

    thread_two_pid = thread_create(stack_two, THREAD_STACKSIZE_MAIN,
                                   THREAD_PRIORITY_MAIN - 2, CREATE_STACKTEST,
                                   thread_two, NULL, "thread_two");

    return 0;
}