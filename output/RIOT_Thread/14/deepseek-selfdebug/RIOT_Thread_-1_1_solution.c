
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define THREAD_STACKSIZE 512
#define MSG_QUEUE_SIZE 10

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    (void)arg;
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
    (void)arg;
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg_queue[0]);
        printf("Received message with value: %d\n", msg_queue[0].content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize GPIO for LED blinking (optional, for debugging purposes)
    gpio_init(GPIO_PIN(0, 16), GPIO_OUT);

    // Create sender thread with higher priority
    thread_create(
        thread_stack, THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN + 1,
        THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender");

    // Create receiver thread with lower priority
    thread_create(
        thread_stack, THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        receiver_thread, NULL, "receiver");

    // Main thread will also run, but it's not necessary for this example
    while (1) {
        gpio_toggle(GPIO_PIN(0, 16)); // Toggle LED every second
        xtimer_sleep(1);
    }

    return 0;
}
