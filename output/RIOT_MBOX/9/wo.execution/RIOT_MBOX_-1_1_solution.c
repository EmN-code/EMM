#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "ztimer.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *_sender_thread(void *arg) {
    (void) arg;
    msg_t msg;

    while (1) {
        msg_send(&msg, thread_getpid());
        printf("Message sent!\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, _sender_thread, NULL, "sender");

    msg_t msg;
 
    while (1) {
        msg_receive(&msg);
        printf("Message received!\n");
    }

    return 0;
}