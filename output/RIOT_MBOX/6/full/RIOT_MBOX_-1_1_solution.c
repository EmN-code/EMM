#include <ztimer.h>      // Include ztimer for timing functionality
#include <msg.h>         // Include msg for message box functionality
#include <stdio.h>       // Include standard I/O for printing

#define MAIN_QUEUE_SIZE     (8)     // Define the size of the message queue
#define INTERVAL            (1000U) // Define the interval period for sending messages in milliseconds

static msg_t main_msg_queue[MAIN_QUEUE_SIZE]; // Define the message queue

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);  // Sleep for INTERVAL milliseconds
        msg_send(&msg, thread_getpid());      // Send a message to self, simulating an event
        puts("Message sent!");                 // Print message sent
    }
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);  // Initialize the message queue
    
    // Start the sender thread
    kernel_pid_t pid = thread_create(
        sender_thread,
        NULL,
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        "sender"
    );
    
    msg_t msg;
    while (1) {
        msg_receive(&msg);  // Block until a message is received
        puts("Message received!");  // Print message received
    }
    return 0;
}