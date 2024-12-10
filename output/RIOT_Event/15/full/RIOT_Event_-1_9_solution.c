#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "pthread.h"
#include "msg.h"
#include "mutex.h"
#include "event/thread.h"

#define PERIODIC_INTERVAL 1000000 // 1 second in microseconds

static void callback_function(void *arg) {
    (void)arg;
    printf("Periodic callback executed.\n");
}

int main(void) {
    // Initialize the periodic timer
    static ztimer_t periodic_timer;
    ztimer_set_msg(&ztimer_usec, &periodic_timer, PERIODIC_INTERVAL, msg_receive, NULL, 0);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        printf("Main loop iteration.\n");
    }
    return 0;
}