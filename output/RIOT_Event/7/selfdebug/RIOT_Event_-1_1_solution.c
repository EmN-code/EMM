#include <stdio.h>
#include <thread/thread.h>
#include <event/periodic.h>

void callback(void *arg) {
    printf("Timer triggered!\n");
}

int main(void) {
    tim_t my_timer;
    event_periodic_t my_periodic_event;
    
    my_timer.callback = callback; // Set the callback function
    my_timer.arg = NULL;
    timer_set_periodic(my_timer, 0, 1000000, 0); // 1 second = 1000000 microseconds

    while (1) {
        thread_usleep(1000000); // Sleep the main thread to save CPU resources
    }
    
    return 0;
}