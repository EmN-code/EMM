
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIORITY     (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL_MS         (1000000U)  // 1 second

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static char sender_stack[SENDER_STACK_SIZE];
static kernel_pid_t sender_pid;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0x1234;

    while (1) {
        xtimer_sleep(INTERVAL_MS / 1000000U);
        msg_send(&msg, sender_pid);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIORITY,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with type %d and value %d\n", msg.type, msg.content.value);
    }

    return 0;
}
