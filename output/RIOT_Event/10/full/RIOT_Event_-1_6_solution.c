#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>

#define DELAY 1U

void callback_function(void *arg)
{
    (void)arg;  // Suppress unused variable warnings
    printf("Callback function is called every second!\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_init(&evtimer);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, DELAY * 1000); // Sleep for 1 second
        callback_function(NULL);
    }
    return 0;
}