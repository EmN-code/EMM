
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <time_units.h>
#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <esp_system.h>
#include <irq.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define STACKSIZE 1024
#define PRIORITY 5

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_msg_t evtimer;

static void event_handler(void *arg)
{
    printf("Event handled\n");
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    evtimer_init_mbox(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = event_handler;
    event.event.arg = NULL;

    evtimer_add_msg(&evtimer, &event, KERNEL_PID_UNDEF);

    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Message received\n");
        }
    }

    return 0;
}
