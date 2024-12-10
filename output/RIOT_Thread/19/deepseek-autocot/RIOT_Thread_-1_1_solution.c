
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    msg_t msg;
    msg.content.value = 42; // Example message content

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

    while (1) {
        msg_receive(&msg_queue[0]);
        printf("Received message with value: %d\n", msg_queue[0].content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize the GPIOs
    gpio_init(GPIO_PIN(0, 16), GPIO_OUT);
    gpio_init(GPIO_PIN(0, 17), GPIO_OUT);

    // Create the sender thread with high priority
    thread_create(
        stack_buffer, sizeof(stack_buffer),
        PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender");

    // Create the receiver thread with low priority
    thread_create(
        stack_buffer, sizeof(stack_buffer),
        PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        receiver_thread, NULL, "receiver");

    // Get the PIDs of the created threads
    sender_pid = thread_getpid();
    receiver_pid = thread_getpid();

    // Main loop
    while (1) {
        gpio_set(GPIO_PIN(0, 16));
        xtimer_usleep(100000); // 100ms
        gpio_clear(GPIO_PIN(0, 16));
        xtimer_usleep(900000); // 900ms
    }

    return 0;
}
