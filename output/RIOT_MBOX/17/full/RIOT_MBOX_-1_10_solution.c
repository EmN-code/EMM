#include <xtimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <msg.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <esp_system.h>
#include <kernel_defines.h>
#include <thread.h>
#include <event/thread.h>
#include <pthread.h>
#include <time_units.h>
#include <timex.h>
#include <ztimer.h>
#include <esp_wifi.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <dht_params.h>
#include <evtimer.h>
#include "mbox.h"

#define STACK_SIZE (thread_stacksize)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MBOX_SIZE (8)

char my_thread_stack[STACK_SIZE];

mbox_t mbox;
msg_t mbox_queue[MBOX_SIZE];

void *thread_handler(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        mbox_get(&mbox,msg);
        printf("Received message with type: %d\n", msg.type);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    mbox_init(&mbox, mbox_queue, MBOX_SIZE);
    pthread_t thread;
    if (pthread_create(&thread, NULL, thread_handler, NULL) != 0) {
        printf("Error: Thread could not be created\n");
        return 1;
    }

    msg_t msg;
    msg.type = 1;
    while (1) {
        mbox_send(&mbox,msg);
        printf("Sent message with type: %d\n", msg.type);
        xtimer_sleep(2);
    }

    return 0;
}
