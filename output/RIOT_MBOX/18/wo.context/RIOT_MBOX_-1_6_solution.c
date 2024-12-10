#include <xtimer.h>
#include <msg.h>
#include <event.h>
#include <stdio.h>

#define NUM_OF_MSGS 8

static msg_t msg_queue[NUM_OF_MSGS];
static mbox_t mbox;

void send_event(void)
{
    msg_t msg;
    msg.content.value = 1;  // example message
    if (mbox_post(&mbox, &msg) < 0) {
        printf("Failed to send message\n");
    } else {
        printf("Message sent\n");
    }
}

void *xmitter_thread(void *arg)
{
    (void)arg;

    while (1) {
        send_event();
        xtimer_sleep(2);  // send every 2 seconds
    }
    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, NUM_OF_MSGS);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_WOUT_YIELD, xmitter_thread, NULL, "xmitter");

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);  // blocks until message is available
        printf("Received message with value: %ld\n", msg.content.value);
    }
    return 0;
}