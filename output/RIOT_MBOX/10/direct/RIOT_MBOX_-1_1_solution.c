#include <stdio.h>
#include <msg.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 8
#define INTERVAL (1U * US_PER_SEC)

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;

    while (1) {
        msg.content.value = counter++;
        msg_send(&msg, thread_getpid());
        printf("Sender: Sent message with value: %d\n", msg.content.value);

        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    puts("Message Box Example started.");

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(sender_thread, NULL, THREAD_STACKSIZE_DEFAULT,
                                            THREAD_PRIORITY_MAIN - 1, "sender");
    
    kernel_pid_t receiver_pid = thread_create(receiver_thread, NULL, THREAD_STACKSIZE_DEFAULT,
                                              THREAD_PRIORITY_MAIN - 2, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}