#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE 8
#define SENDER_STACKSIZE THREAD_STACKSIZE_MAIN
#define SENDER_PRIO THREAD_PRIORITY_MAIN - 1

static char sender_stack[SENDER_STACKSIZE];
static kernel_pid_t main_thread_pid;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 123;
    msg.type = 0xA0;
    
    while (1) {
        msg_send(&msg, main_thread_pid);
        printf("Message sent from thread %d\n", thread_getpid());
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    msg_t main_msg_queue[MAIN_QUEUE_SIZE];
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    main_thread_pid = thread_getpid();
    
    thread_create(sender_stack, SENDER_STACKSIZE, SENDER_PRIO,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Message received with content: %lu\n", msg.content.value);
    }

    return 0;
}