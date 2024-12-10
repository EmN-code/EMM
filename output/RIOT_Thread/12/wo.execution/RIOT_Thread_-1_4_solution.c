#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <time_units.h>
#include <xtimer.h>
#include <msg.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

char stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];

void *thread_func(void *arg) {
    msg_t msg;
    msg_receive(&msg);
    printf("Received message: type=%d, value=%lu\n", msg.type, msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, NULL);
    
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    msg_send(&msg, thread_getpid_of_name("thread_func"));
    
    pthread_join(thread, NULL);
    return 0;
}