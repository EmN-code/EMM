#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"

void event_handler(void *arg) {
    (void)arg; // To avoid unused parameter warning
    printf("Event triggered!\n");
}

int main(void) {
    ztimer_t timer;
    ztimer_init(ZTIMER_SEC, &timer, event_handler, NULL, 1);
    ztimer_set(ZTIMER_SEC, &timer, 1);

    while (1) {
        usleep(1000); // Reduce CPU usage
    }
    return 0;
}