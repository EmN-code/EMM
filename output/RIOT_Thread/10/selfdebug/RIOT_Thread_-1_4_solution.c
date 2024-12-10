#include <thread.h>  // Import thread.h for threading functionalities
#include <xtimer.h>  // Import xtimer.h for using xtimer_sleep function

#define THREAD_STACKSIZE_LARGE 2048  // Define THREAD_STACKSIZE_LARGE if not defined
#define MSG_QUEUE_SIZE 8  // Define size of message queue

// Message structure
typedef struct {
    int value;
} msg_t;

// Thread data structure and stacks
static char stack[2][THREAD_STACKSIZE_LARGE];

// Message queues
static msg_t msg_queue_1[MSG_QUEUE_SIZE];
static msg_t msg_queue_2[MSG_QUEUE_SIZE];

// Thread function that sends messages
void *sender_thread(void *arg) {
    msg_t msg;
    msg.value = 42;  // Example message
    while (1) {
        msg_send(&msg, thread_getpid());  // Send message to receiver
        xtimer_sleep(1);  // Delay using xtimer_sleep
    }
    return NULL;
}

// Thread function that receives messages
void *receiver_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);  // Receive message
        printf("Received message with value: %d\n", msg.value);  // Print received message
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(
        stack[0], THREAD_STACKSIZE_LARGE, THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender"
    );
    
    kernel_pid_t receiver_pid = thread_create(
        stack[1], THREAD_STACKSIZE_LARGE, THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver"
    );

    printf("Sender thread PID: %d, Receiver thread PID: %d\n", sender_pid, receiver_pid);

    return 0;
}