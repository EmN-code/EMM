#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "msg.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "kernel_defines.h"

#define INTERVAL (1U * 1000000U) // 1 second in microseconds

static void _callback(void *arg)
{
    (void)arg;
    printf("Callback function called.\n");
}

int main(void)
{
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    ztimer_t timer;
    timer.callback = _callback;
    timer.arg = NULL;
    
    while (1) {
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
        timer.callback(timer.arg); 
    }

    return 0;  
}