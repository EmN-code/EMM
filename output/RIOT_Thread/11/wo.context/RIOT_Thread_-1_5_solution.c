#include <stdio.h>
#include <msg.h>
#include <thread.h>

#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_RECEIVER THREAD_STACKSIZE_DEFAULT
#define THREAD_STACKSIZE_SENDER   THREAD_STACKSIZE_DEFAULT

static char receiver_thread_stack[THREAD_STACKSIZE_RECEIVER];
static char sender_thread_stack[THREAD_STACKSIZE_SENDER];

// Message queues
static msg_t receiver_queue[32];

// Receiver function prototype
typedef struct {
    kernel_pid_t pid;
    char name[16];
} thread_info_t;

// Function to print thread information
void print_thread_info(thread_info_t *info) {
    printf("Thread PID: %d, Name: %s\n", info->pid, info->name);
}

// Receiver thread function
void *receiver(void *arg) {
    (void)arg; 
    msg_t msg;
    thread_info_t receiver_info = {.pid = thread_getpid(), .name = "Receiver"};
    print_thread_info(&receiver_info);
    msg_init_queue(receiver_queue, 32);
    while (1) {
        msg_receive(&msg);
        printf("Receiver: got message with content: %ld\n", msg.content.value);
    }
    return NULL;
}

// Sender thread function
void *sender(void *arg) {
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    thread_info_t sender_info = {.pid = thread_getpid(), .name = "Sender"};
    print_thread_info(&sender_info);
    while (1) {
        msg_t msg;
        msg.content.value = 42; // Example message
        msg_send(&msg, receiver_pid);
        printf("Sender: sent message to Receiver\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid;

    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver, NULL, "receiver");

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_SENDER, 0,
                  sender, &receiver_pid, "sender");

    return 0;
}
