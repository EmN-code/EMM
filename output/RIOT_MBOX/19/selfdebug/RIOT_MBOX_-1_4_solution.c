#include <xtimer.h>
#include <sys/msg.h>
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t main_queue[MAIN_QUEUE_SIZE];

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_pool[4];
static mbox_t mbox;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t *msg_ptr = &msg;
    
    while (1) {
        xtimer_sleep(1);
        msg.content.value = xtimer_now_usec();
        if (mbox_try_put(&mbox, msg_ptr) != 1) {
            printf("[Sender] Message box is full, could not send message.\n");
        } else {
            printf("[Sender] Message sent with timestamp: %ld\n", msg.content.value);
        }
    }
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t *msg_ptr = &msg;
    
    while (1) {
        if (mbox_get(&mbox, msg_ptr, 1) != 1) {
            printf("[Receiver] No message received.\n");
        } else {
            printf("[Receiver] Message received with timestamp: %ld\n", msg.content.value);
        }
    }
}

int main(void) {
    msg_init_queue(main_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, msg_pool, 4);

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0, receiver, NULL, "receiver");
    
    return 0;
}