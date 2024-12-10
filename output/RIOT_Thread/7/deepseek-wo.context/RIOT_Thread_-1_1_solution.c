
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t sender_pid, receiver_pid;

void *sender_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;
    msg.content.value = 123; // Example message content

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1); // Send a message every second
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize GPIO for LED blinking (optional, for debugging purposes)
    gpio_init(GPIO_PIN(0, 16), GPIO_OUT);

    // Create sender thread with high priority
    thread_create(
        stack_buffer, sizeof(stack_buffer),
        PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender");

    // Create receiver thread with low priority
    thread_create(
        stack_buffer, sizeof(stack_buffer),
        PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        receiver_thread, NULL, "receiver");

    // Main thread can also do some work or just sleep
    while (1) {
        gpio_toggle(GPIO_PIN(0, 16)); // Toggle LED
        xtimer_sleep(2); // Sleep for 2 seconds
    }

    return 0;
}
