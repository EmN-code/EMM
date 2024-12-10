#include <msg.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <xtimer.h> 
#include <thread.h> 

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)
#define INTERVAL            (1U * US_PER_SEC)  /* 1 second interval */

msg_t _msg_queue[8];

static char sender_stack[THREAD_STACKSIZE_MAIN];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    puts("Sender started\n");

    while (1) {
        /* wait INTERVAL seconds */
        xtimer_sleep(INTERVAL / US_PER_SEC);

        /* send message */
        msg.content.value = 42;
        msg_send(&msg, thread_getpid());
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(_msg_queue, 8);

    puts("Receiver started\n");

    while (1) {
        msg_receive(&msg);
        printf("Received message with data: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    puts("RIOT message box example\n");

    /* Launch sender thread */
    thread_create(sender_stack, sizeof(sender_stack),
                  SENDER_PRIO, 0, sender, NULL, "sender");

    /* Start receiver function directly in the main thread */
    receiver(NULL);

    return 0;
}