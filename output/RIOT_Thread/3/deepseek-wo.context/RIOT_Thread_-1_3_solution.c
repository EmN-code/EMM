
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/uart.h"
#include "esp_log.h"

#define MAIN_QUEUE_SIZE     (8)
#define PRIORITY_HIGH       (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW        (THREAD_PRIORITY_MAIN + 1)
#define STACK_SIZE          (THREAD_STACKSIZE_DEFAULT)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static char stack_high[STACK_SIZE];
static char stack_low[STACK_SIZE];

static void *thread_high(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        ESP_LOGI("THREAD_HIGH", "Received message: %d", (int)msg.content.value);
    }

    return NULL;
}

static void *thread_low(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        ESP_LOGI("THREAD_LOW", "Received message: %d", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                                          THREAD_CREATE_STACKTEST, thread_high, NULL, "thread_high");

    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                                         THREAD_CREATE_STACKTEST, thread_low, NULL, "thread_low");

    msg_t msg;
    msg.type = 0;

    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        msg_send(&msg, pid_high);
        xtimer_sleep(1);
        msg_send(&msg, pid_low);
        xtimer_sleep(1);
    }

    return 0;
}
