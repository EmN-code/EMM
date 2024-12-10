#include <ztimer.h>
#include <msg.h>
#include <event/callback.h>

#define MAIN_QUEUE_SIZE     (8)
#define MBOX_SIZE           (32)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static msg_t _mbox_queue[MBOX_SIZE];
static mbox_t _mbox;

void sender_thread(void) {
    msg_t msg;
    msg.content.value = 1;
    while(1) {
        msg_send(&msg, mbox_get(&msg), 0);
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&_mbox, _mbox_queue, MBOX_SIZE);
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            THREAD_PRIORITY_MAIN - 1, 0,
                                            sender_thread, NULL, "sender");
    printf("Message box initialized and sender thread started.\n");
    return 0;
}