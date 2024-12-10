#include <stdio.h>  // Standard input/output definitions
#include <string.h>  // String function definitions
#include <msg.h>  // RIOT's message API
#include <xtimer.h>  // Timer API to manage time-based events

#define MAIN_QUEUE_SIZE (8)  // Define the size of the message queue
#define INTERVAL_USEC (1000000)  // Interval for periodic events in microseconds

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];  // Message queue

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    
    while (1) {
        msg.content.value++;
        printf("Sending message with value: %d\n", msg.content.value);
        msg_send(&msg, thread_getpid());
        xtimer_usleep(INTERVAL_USEC);  // Wait for the defined interval
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        msg_receive(&msg);  // Receive message from the queue
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("RIOT ESP32 Message Box Example\n");
    
    // Initialize the message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    
    // Start the sender and receiver threads
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), 
                                           THREAD_PRIORITY_MAIN - 1, 0, 
                                           sender, NULL, "sender");
    printf("Sender thread PID: %d\n", sender_pid);
    
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), 
                                              THREAD_PRIORITY_MAIN - 1, 0, 
                                              receiver, NULL, "receiver");
    printf("Receiver thread PID: %d\n", receiver_pid);
    
    return 0;
}