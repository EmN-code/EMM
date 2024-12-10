#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

#define SENDER_THREAD_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define RECEIVER_THREAD_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define SENDER_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIO (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[SENDER_THREAD_STACKSIZE];
static char receiver_stack[RECEIVER_THREAD_STACKSIZE];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message content
    
    while(1) {
        printf("Sender: Sending message with value %lu\n", msg.content.value);
        if (msg_send(&msg, receiver_pid) <= 0) {
            printf("Sender: Message send failed\n");
        } else {
            printf("Sender: Message sent successfully\n");
        }
        xtimer_sleep(1);  // Send message every second
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    
    while(1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: Received message with value %lu\n", msg.content.value);
        }
        xtimer_usleep(500000);  // Check for messages every 500 ms
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, RECEIVER_THREAD_STACKSIZE,
                                 RECEIVER_THREAD_PRIO, 0,
                                 receiver_thread, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create receiver thread\n");
        return 1;
    }

    sender_pid = thread_create(sender_stack, SENDER_THREAD_STACKSIZE,
                               SENDER_THREAD_PRIO, 0,
                               sender_thread, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create sender thread\n");
        return 1;
    }

    while(1) {
        // Placeholder for main logic
        xtimer_sleep(3);
    }

    return 0;
}