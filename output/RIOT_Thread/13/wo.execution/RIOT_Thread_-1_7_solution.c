#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread.h"
#include "timex.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "paho_mqtt.h"

#define MAIN_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MAIN_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8]; /* Ensure it's a power of two */

void *thread_function(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Thread received message: %u\n", msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    kernel_pid_t thread_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        MAIN_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
        thread_function, NULL, "receiver_thread"
    );

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }

    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    msg_send(&msg, thread_pid);

    return 0;
}