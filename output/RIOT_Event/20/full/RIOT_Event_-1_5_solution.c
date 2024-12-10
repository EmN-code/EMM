#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdio.h"
#include "netinet/in.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"

#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];

void callback_function(void)
{
    printf("Callback executed!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_msg_event_t event;
    evtimer_msg_init(&event);  // Correct initialization
    evtimer_msg_set(&event, callback_function);
    
    while (1) {
        evtimer_msg_set_every(&event, XTIMER_MSEC(1000));  // Executes every second
        msg_receive(&event.msg);
        event.callback();
    }

    return 0;
}