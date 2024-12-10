#include <xtimer.h>
#include <stdio.h>
#include <pthread.h>
#include <msg.h>

#define HIGH_PRIORITY 3
#define LOW_PRIORITY 5

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];  // This is a message queue of size 8 to hold the messages.

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg, reply;

    while(1) {
        // Receive message
        msg_receive(&msg);
        printf("High priority thread received message\n");

        // Reply to the received message
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
        printf("High priority thread replied\n");
    }

    return NULL;
}

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg, reply;

    while (1) {
        msg.content.value = 42;
        msg_send_receive(&msg, &reply, thread_getpid_of(&thread2));
        printf("Low priority thread sent message and received reply: %d\n", reply.content.value);

        xtimer_sleep(2);
    }

    return NULL;
}

int main(void) {
    pthread_t thread1, thread2;

    msg_init_queue(msg_queue, 8);

    // Create high priority thread
    if (pthread_create(&thread1, NULL, high_priority_thread, NULL) != 0) {
        printf("Failed to create high priority thread\n");
    }
    pthread_setschedprio(thread1, HIGH_PRIORITY);

    // Create low priority thread
    if (pthread_create(&thread2, NULL, low_priority_thread, NULL) != 0) {
        printf("Failed to create low priority thread\n");
    }
    pthread_setschedprio(thread2, LOW_PRIORITY);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}