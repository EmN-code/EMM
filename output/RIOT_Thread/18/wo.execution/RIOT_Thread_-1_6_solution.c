#include "xtimer.h"
#include "event/thread.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define THREAD_PRIO_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE     (THREAD_STACKSIZE_MAIN)

static char sender_stack[THREAD_STACKSIZE];

msg_t msg_queue[8];
msg_t msg;

void *sender_thread(void *arg) {
    (void)arg; // argument is not used

    msg.content.value = 42; // example message content

    while (1) {
        msg_send(&msg, thread_getpid()); // send message to main thread
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIO_SENDER, 
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", msg.content.value);
    }

    return 0;
}