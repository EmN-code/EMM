#include <ztimer.h>
#include <stdio.h>
#include <msg.h>

static ztimer_t my_timer;

void callback(void *arg) {
    (void)arg;
    printf("Timer Callback Triggered\n");
}

int main(void) {
    ztimer_init(); // Initialize timers
    
    // Initialize the timer and specify the callback
    ztimer_set_msg(&my_timer, ZTIMER_MSEC, 1000, &msg_in, THREAD_PID_MAIN);

    // Arm (start) the timer
    ztimer_start();
    
    while(1) {
        msg_receive(&msg_in);
        // Other application code can run here
    }
    return 0;
}