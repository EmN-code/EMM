
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/uart.h"

#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static kernel_pid_t pid_high;
static kernel_pid_t pid_low;

static msg_t msg_queue_high[8];
static msg_t msg_queue_low[8];

void *thread_high(void *arg)
{
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue_high, 8);

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue_low, 8);

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize UART for debugging
    uart_init(UART_DEV(0), 115200, NULL, NULL);

    // Create high priority thread
    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high");

    // Create low priority thread
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low");

    // Send messages to threads
    msg_t msg;
    msg.content.value = 1;
    msg_send(&msg, pid_high);

    msg.content.value = 2;
    msg_send(&msg, pid_low);

    // Sleep to allow threads to process messages
    xtimer_sleep(1);

    return 0;
}
