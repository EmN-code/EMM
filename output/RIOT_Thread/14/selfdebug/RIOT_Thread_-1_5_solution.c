#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN  (1024)
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN - 1)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

msg_t msg_queue_one[8];
msg_t msg_queue_two[8];

void *thread_one(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_one, 8);
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        puts("Thread One: Sending message.");
        msg_send(&msg, thread_getpid());
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_two, 8);
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Thread Two: Received message with value %ld.\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN + 1, 0, thread_one, NULL, "thread_one");
    thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN + 2, 0, thread_two, NULL, "thread_two");
    return 0;
}