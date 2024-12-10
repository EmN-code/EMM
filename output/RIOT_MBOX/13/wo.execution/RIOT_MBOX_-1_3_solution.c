#include <msg.h>
#include <net/nanocoap/cache.h>
#include <xtimer.h>
#include <event/callback.h>
#include <stddef.h>
#include <sys/bytes.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)
#define QUEUE_SIZE (8)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t my_mbox;

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        xtimer_sleep(1);
        msg.content.value++;
        printf("Sending message with value: %ld\n", msg.content.value);
        mbox_try_put(&my_mbox, &msg);
    }
}

void receiver_thread(void)
{
    msg_t msg;

    while (1) {
        mbox_get(&my_mbox, &msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }
}

int main(void)
{
    printf("Initializing mbox example\n");
    mbox_init(&my_mbox, msg_queue, QUEUE_SIZE);

    kernel_pid_t pid_send = thread_create(sender_thread, NULL, THREAD_STACKSIZE_DEFAULT, 
                                          THREAD_PRIORITY_MAIN - 1, "sender");
    
    kernel_pid_t pid_recv = thread_create(receiver_thread, NULL, THREAD_STACKSIZE_DEFAULT, 
                                          THREAD_PRIORITY_MAIN - 2, "receiver");
                                          
    (void) pid_send;
    (void) pid_recv;

    return 0;
}