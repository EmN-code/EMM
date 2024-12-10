#include <thread.h>
#include <msg.h>

#define STACKSIZE_MAIN 1024  // define stack size for main thread
#define STACKSIZE_MSG 1024   // define stack size for message handling thread
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN-1)  // main thread priority
#define THREAD_PRIORITY_MSG THREAD_PRIORITY_MAIN  // message thread priority

static char receiver_stack[STACKSIZE_MAIN];
static char sender_stack[STACKSIZE_MSG];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

msg_t msg_queue[8];  // queue to hold messages

void *receiver_thread(void *arg) {
    msg_t msg;
    (void)arg;
    
    while (1) {
        msg_receive(&msg);  // blocking receive
        printf("Received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *sender_thread(void *arg) {
    msg_t msg;
    (void)arg;

    msg.content.value = 42;  // the message content

    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Sent message: %d\n", msg.content.value);
        thread_yield();  // yield to other threads
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    receiver_pid = thread_create(
        receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MSG,
        THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    sender_pid = thread_create(
        sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    printf("Threads initialized: sender PID: %d, receiver PID: %d\n", sender_pid, receiver_pid);
    
    return 0;
}