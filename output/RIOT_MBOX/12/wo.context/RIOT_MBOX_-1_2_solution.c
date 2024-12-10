#include <xtimer.h>  // Required for timer functionalities
#include <msg.h>     // Required for message functionalities
#include <kernel.h>  
#include <event.h>   // Event handling functions

#define MESSAGE_QUEUE_SIZE   (8U)  // Define the size of the message queue
#define THREAD_PRIORITY      (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t event_thread_pid;
static msg_t event_msg_queue[MESSAGE_QUEUE_SIZE];
static char event_thread_stack[THREAD_STACKSIZE_MAIN];

// Function for the event thread
void *event_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(event_msg_queue, MESSAGE_QUEUE_SIZE);
    
    while (1) {
        if (msg_receive(&msg)) {
            printf("Received message with content: %ld\n", msg.content.value);
            // Process the received message (event)
        }
    }
    return NULL;
}

// Main function
int main(void) {
    printf("Message box example on RIOT with ESP32\n");
    
    event_thread_pid = thread_create(
        event_thread_stack, sizeof(event_thread_stack),
        THREAD_PRIORITY, 0,
        event_thread, NULL, "event_thread");

    msg_t msg;
    msg.content.value = 1;
    while (1) {
        printf("Sending message with content: %ld\n", msg.content.value);
        msg_send(&msg, event_thread_pid);
        xtimer_sleep(1);  // Send a message every second
    }
    return 0;
}