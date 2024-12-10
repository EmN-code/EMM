#include "ztimer.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"
#include "stdint.h"
#include "netinet/in.h"
#include "msg.h"
#include "mutex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "kernel_defines.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER  (THREAD_PRIORITY_MAIN - 2)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int i = 0;

    while (1) {
        msg.type = i;
        msg.content.value = i;
        if (msg_send(&msg, thread_getpid_of(NULL)) <= 0) {
            printf("Failed to send message\n");
        }
        i++;
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u, value: %u\n", msg.type, msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_SENDER, 0, sender_thread, NULL, "sender");
    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, receiver_thread, NULL, "receiver");
    return 0;
}
