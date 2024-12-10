#include <xtimer.h>
#include <msg.h>
#include <evtimer.h>
#include <stdio.h>

#define INTERVAL (1000000U) // Set interval to 1 second

static evtimer_t evtimer;
static msg_t msg_queue[8];
static evtimer_event_t event;
static kernel_pid_t main_pid;

void callback_function(void *arg) {
    (void)arg;
    msg_t msg;

    // Prepare message
    msg.content.value = 42;
    printf("Sending message: %d\n", msg.content.value);

    // Send message
    msg_send(&msg, main_pid);
}

int main(void) {
    main_pid = thread_get_unchecked(kernel_pid_t);

    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    evtimer_init(&evtimer, callback_function, NULL);

    evtimer_set(&evtimer, INTERVAL);
    evtimer_add_mbox(&evtimer, &event, main_pid);

    puts("Mailbox setup with periodic event sending.\n");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }

    return 0;
}