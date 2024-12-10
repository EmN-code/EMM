#include <xtimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <timex.h>
#include <msg.h>
#include <mbox.h>

#define INTERVAL 1U /* 1 second */

static void _callback(void *arg) {
    (void)arg;  /* avoid unused parameter warning */
    puts("Callback triggered!");
}

int main(void) {
    /* Variables for timer */
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        /* Wait for next interval */
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL * US_PER_SEC);
        
        /* Execute callback */
        _callback(NULL);
    }
    
    return 0;
}