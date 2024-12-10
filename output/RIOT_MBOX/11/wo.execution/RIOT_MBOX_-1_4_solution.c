#include <stdio.h>  // Required for printf
#include <msg.h>   // RIOT's messaging interface
#include <xtimer.h> // Timer for periodic events

#define QUEUE_SIZE    (8)
#define SEND_INTERVAL (1000000) // 1 second in microseconds

static msg_t msg_queue[QUEUE_SIZE]; // Message queue buffer

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    while (1) {
        printf("Sending message %d\n", msg.content.value);
        msg_send(&msg, sched_active_pid);
        msg.content.value++;
        xtimer_usleep(SEND_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    printf("Message queue initialized.\n");
    kernel_pid_t sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                                            THREAD_PRIORITY_MAIN - 1, 0,
                                            sender_thread, NULL, "sender");
    receiver_thread(NULL);
    return 0;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message %d\n", msg.content.value);
    }
    return NULL;
}