#include <ztimer.h>
#include <stdio.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_INTERVAL     (1U * ZTIMER_MSEC_PER_SEC)  // 1 second interval

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void sender_loop(void) {
    msg_t msg;
    msg.content.value = 42;  // Arbitrary value for demonstration

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, SENDER_INTERVAL);
        if (msg_send(&msg, thread_getpid()) <= 0) {
            puts("Failed to send message");
        } else {
            puts("Message sent");
        }
    }
}

int main(void) {
    puts("RIOT Message Box Example");
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    sender_loop();

    return 0;
}