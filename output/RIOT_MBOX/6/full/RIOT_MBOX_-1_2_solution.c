#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define INTERVAL (1 * US_PER_SEC)

static void *sender_thread(void *arg) {
    (void)arg;
    evtimer_mbox_t mevent;
    msg_t msg, reply;
    msg.content.value = 10;
    
    evtimer_mbox_init(&mevent);
    evtimer_mbox_add(&mevent, INTERVAL);

    while (1) {
        evtimer_mbox_wait(&mevent);
        msg_send_receive(&msg, &reply, KERNEL_PID_MAIN);
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t m;
 
    while (1) {
        msg_receive(&m);
        printf("Received message with value: %" PRIu32 "\n", m.content.value);
        msg_reply(&m, &m);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];

    thread_create(sender_stack, sizeof(sender_stack), 
                  THREAD_PRIORITY_MAIN - 1, 0, sender_thread, 
                  NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), 
                  THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, 
                  NULL, "receiver");

    return 0;
}
