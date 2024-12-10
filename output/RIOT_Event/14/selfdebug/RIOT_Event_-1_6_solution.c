#include <ztimer.h> 
#include <stdio.h> 

void event_callback(void *arg)
{
    (void)arg;  // Prevent unused variable warning
    puts("Event triggered every second.");
}

int main(void)
{
    ztimer_t timer;
    ztimer_init(&timer);
    ztimer_set_msg(&timer, ZTIMER_MSEC, ztimer_now(ZTIMER_MSEC) + 1000, NULL);
    ztimer_set_cb(&timer, event_callback, NULL);
    
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep for 1 second
    }
    return 0;
}