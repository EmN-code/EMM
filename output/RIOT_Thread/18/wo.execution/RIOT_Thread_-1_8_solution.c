#include "msg.h"
#include "time_units.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "stdbool.h"
#include "net/sock/udp.h"
#include "stdio.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "sys/socket.h"
#include "xtimer.h"
#include "net/sock/util.h"
#include "dht.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "mbox.h"
#include "event/thread.h"
#include "net/emcute.h"
#include "timex.h"
#include "netinet/in.h"
#include "mma8x5x.h"
#include "unistd.h"
#include "mma8x5x_params.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "stdatomic.h"

#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE_SENDER         (THREAD_STACKSIZE_DEFAULT)
#define STACKSIZE_RECEIVER       (THREAD_STACKSIZE_DEFAULT)

static char stack_sender[STACKSIZE_SENDER];
static char stack_receiver[STACKSIZE_RECEIVER];

static msg_t queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message content
    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);  // Send message every second (example timing)
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    kernel_pid_t sender_pid = thread_create(stack_sender, STACKSIZE_SENDER,
                                            THREAD_PRIORITY_SENDER, 0,
                                            sender, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating sender thread\n");
        return 1;
    }

    kernel_pid_t receiver_pid = thread_create(stack_receiver, STACKSIZE_RECEIVER,
                                              THREAD_PRIORITY_RECEIVER, 0,
                                              receiver, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread\n");
        return 1;
    }

    // Let the threads do their jobs
    thread_yield_higher();

    return 0;
}