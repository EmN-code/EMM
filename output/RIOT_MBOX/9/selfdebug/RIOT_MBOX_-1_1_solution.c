#include <ztimer.h>  // Include ztimer for timer functionalities
#include <string.h>   // Include string for memory operations
#include <msg.h>   // Include messaging functions for RIOT OS
#include <stdio.h>   // Include standard I/O for printf function

#define INTERVAL 1000000U  // Interval in microseconds (1 second) for the timer
#define MAIN_QUEUE_SIZE 8  // Size of the main message queue

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];  // Define the main message queue

// Function to handle timer events
void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Timer event triggered!\n");
    // Here you can send a message or perform other tasks.
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);  // Initialize the message queue
    
    ztimer_t timer;  // Declare a ztimer_t structure for the timer
    ztimer_set_msg(ZTIMER_USEC, &timer, INTERVAL, &msg_t{}, 0);  // Set the timer

    while (1) {
        msg_t msg;
        msg_receive(&msg);  // Receive any incoming messages and handle accordingly
        timer_callback(NULL);  // Call the timer callback manually to simulate periodic execution
    }

    return 0;
}