#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <mutex.h>
#include <event/timeout.h>  // Assuming event/timeout provides mbox_t

#define INTERVAL (1000000U)
#define MBOX_SIZE (8)

mbox_t my_mbox;
msg_t mbox_buffer[MBOX_SIZE];
ztimer_t my_timer;

void *sender(void *arg) {
    msg_t msg;
    (void)arg;

    while (1) {
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
        printf("Sending message\n");
        msg.content.value = 1;
        mbox_send(&my_mbox, &msg);
    }
    return NULL;
}

void *receiver(void *arg) {
    msg_t msg;
    (void)arg;

    while (1) {
        mbox_receive(&my_mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    mbox_init(&my_mbox, mbox_buffer, MBOX_SIZE);
    ztimer_periodic_start(ZTIMER_USEC, &my_timer, INTERVAL);
    
    kernel_pid_t sender_thread = thread_create(sender_stack, sizeof(sender_stack),
                                                THREAD_PRIORITY_MAIN - 1, 0,
                                                sender, NULL, "sender_thread");

    kernel_pid_t receiver_thread = thread_create(receiver_stack, sizeof(receiver_stack),
                                                  THREAD_PRIORITY_MAIN - 1, 0,
                                                  receiver, NULL, "receiver_thread");
    
    thread_join(sender_thread);
    thread_join(receiver_thread);
    
    return 0;
}