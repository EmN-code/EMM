#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "evtimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "mbox.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "time_units.h"
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval in microseconds

static evtimer_t evtimer;
static msg_t msg_queue[8];
static char stack[THREAD_STACKSIZE_DEFAULT];

static void callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered every second!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, 8);

    evtimer_init_msg(&evtimer);
    evtimer_set(&evtimer, callback, NULL, INTERVAL);
    evtimer_add_msg(&evtimer, msg_queue, 8);

    while (1) {
        usleep(1000000);
    }

    return 0;
}