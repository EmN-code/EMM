#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE_DEFAULT   (THREAD_STACKSIZE_MAIN)
#define SENDER_THREAD_PRIORITY     (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 2)

char sender_stack[THREAD_STACKSIZE_DEFAULT];
char receiver_stack[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue[4];
kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 4);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Message received with type %d and value %d\n", msg.type, msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_send_t sender_pid = thread_getpid();
    
    msg.type = 0; // Define a message type
    msg.content.value = 42; // Some arbitrary value
    while (1) {
        int rc = msg_send(&msg, receiver_pid);
        
        if (rc != 1) {
            printf("Sender: Error sending message. Retrying in 1 second.\n");
            xtimer_sleep(1);
            continue;
        }

        printf("Sender: Message sent with value: %d\n", msg.content.value);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_THREAD_PRIORITY, 0,
                                 receiver_thread, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread\n");
        return 1;
    }

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            SENDER_THREAD_PRIORITY, 0,
                                            sender_thread, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating sender thread\n");
        return 1;
    }

    return 0;
}