#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_DEFAULT (1024)

char sender_stack[THREAD_STACKSIZE_DEFAULT];
char receiver_stack[THREAD_STACKSIZE_DEFAULT];

msg_t msg_queue[8];
msg_t main_msg;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;  // Example message type
    while (1) {
        msg_send(&msg, thread_getpid());  // Send to itself (for illustration)
        puts("Sender: Message sent");
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        msg_receive(&main_msg);
        printf("Receiver: Message received with type %d\n", main_msg.type);
    }
    return NULL;
}

int main(void) {
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIORITY, 0,
                  sender_thread, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIORITY, 0,
                  receiver_thread, NULL, "receiver");
    return 0;
}