#include <xtimer.h>
#include <unistd.h>
#include <thread.h>
#include <timex.h>
#include <string.h>
#include <msg.h>
#include <event/thread.h>
#include <mbox.h>

#define MAIN_QUEUE_SIZE     (8)
#define MSG_WAIT_TIME       (2 * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];

static msg_t _msg_queue[MAIN_QUEUE_SIZE];
static msg_t msg;

/* Message sender thread function */
void *message_sender_thread(void *arg) {
    (void)arg;
    while (1) {
        msg.content.value = 42; // Example value
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_msg_queue, MAIN_QUEUE_SIZE);

    /* Create the message sender thread */
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  message_sender_thread,
                  NULL, "message sender");

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with value: %d\n", msg.content.value);
            xtimer_sleep(MSG_WAIT_TIME);
        }
    }
    return 0;
}