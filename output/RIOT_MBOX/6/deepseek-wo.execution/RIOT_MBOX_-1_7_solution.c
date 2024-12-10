
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <time_units.h>
#include <stdlib.h>
#include <event/thread.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <mbox.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0x1234;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        msg_send_receive(&msg, &msg, thread_getpid());
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        _mbox_get(&mbox, &msg, 1);
        printf("Received message with type %u and value %u\n", msg.type, msg.content.value);
    }

    return 0;
}
