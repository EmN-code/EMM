#include <ztimer.h>
#include <msg.h>
#include <stdio.h>

#define INTERVAL (1000000U) // 1 second

static msg_t msg_queue[8];
static mbox_t mbox;
static ztimer_t timer;

void message_sender(void) {
    while (1) {
        msg_t msg;
        msg.content.value = 42; // Arbitrary message content
        if (mbox_send(&mbox, &msg) != 1) {
            printf("Failed to send message\n");
        } else {
            printf("Message sent: %d\n", msg.content.value);
        }
        ztimer_sleep(ZTIMER_MSEC, INTERVAL / 1000); // Sleep for the interval period
    }
}

void* receiver_thread(void* arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("RIOT message box example\n");
    mbox_init(&mbox, msg_queue, 8);

    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  receiver_thread, NULL, "receiver");

    message_sender();

    return 0;
}