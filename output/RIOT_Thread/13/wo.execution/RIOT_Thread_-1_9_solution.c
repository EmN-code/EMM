#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with content: %u\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    // Create a new thread with a slightly lower priority
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, receiver_thread, NULL, "receiver");

    msg_t msg;
    kernel_pid_t receiver_pid = thread_getpid();
    while (1) {
        msg.content.value = (uint32_t)esp_random();  // Use ESP32's random function
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }
    return 0;
}