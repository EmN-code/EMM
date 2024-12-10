#include <ztimer.h>
#include <stdio.h>

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer callback triggered\n");
}

int main(void) {
    ztimer_set_msg(ZTIMER_MSEC, &timer, 1000, timer_callback, NULL);
    while(1) {
        msg_t msg;
        msg_receive(&msg);
    }
    return 0;
}