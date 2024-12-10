#include <xtimer.h>
#include <event/thread.h>
#include <msg.h>
#include <kernel_defines.h>
#include <stdio.h>  // Only included to print messages as an analogy for debugging.

#define SLEEP_DURATION       (5U * US_PER_SEC)  // 5 seconds in microseconds.
#define GPIO_PIN             (2)
#define MESSAGE_QUEUE_SIZE   (8)

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];

void *dht11_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);
    
    while (1) {
        printf("Fake DHT11 reading: Temperature: %d, Humidity: %d\n", 25, 55);
        xtimer_usleep(SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    static char stack[KERNEL_CONF_STACKSIZE_DEFAULT];
    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            dht11_thread,
                                            NULL, "dht11_thread");

    printf("DHT11 simulation started on GPIO %d\n", GPIO_PIN);
    return 0;
}