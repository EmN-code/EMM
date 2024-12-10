
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define PRIORITY_HIGH 1
#define PRIORITY_LOW 5

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg.content.value = 123; // Example message content
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1); // Send a message every second
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize GPIO for LED blink example
    gpio_init(GPIO_PIN(0, 16), GPIO_OUT);

    // Create sender thread with high priority
    thread_create(
        thread_stack_init(sender_thread, 512),
        "sender",
        THREAD_PRIORITY_MAIN - PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        &sender_pid
    );

    // Create receiver thread with low priority
    thread_create(
        thread_stack_init(receiver_thread, 512),
        "receiver",
        THREAD_PRIORITY_MAIN - PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        &receiver_pid
    );

    // Main thread blinks an LED to indicate system is running
    while (1) {
        gpio_set(GPIO_PIN(0, 16));
        xtimer_usleep(500000); // 500 ms
        gpio_clear(GPIO_PIN(0, 16));
        xtimer_usleep(500000); // 500 ms
    }

    return 0;
}
