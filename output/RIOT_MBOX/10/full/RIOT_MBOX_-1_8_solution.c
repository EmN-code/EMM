#include "ztimer.h"
#include "xtimer.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "mutex.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE        (8)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;
    while (1) {
        msg.content.value = counter++;
        msg_send(&msg, thread_getpid());
        printf("Message sent: %d\n", msg.content.value);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *msg_receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  msg_receiver, NULL, "msg_receiver");

    msg_sender(NULL);

    return 0;
}