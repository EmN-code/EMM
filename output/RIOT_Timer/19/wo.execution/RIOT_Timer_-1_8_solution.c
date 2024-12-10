#include "ztimer.h"
#include "xtimer.h"
#include <thread.h>
#include <stdio.h>
#include <msg.h>
#include <kernel_defines.h>

// Function Prototypes
void *timer_thread(void *arg);

// Define GPIO LED pin
#define LED_PIN GPIO_PIN(PORT_A, 5)

// Message Queue
static msg_t msg_queue[8];

void *timer_thread(void *arg) {
    (void) arg;
    msg_t msg;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 1 * US_PER_SEC);
        printf("Timer triggered, turning on LED.\n");
        gpio_set(LED_PIN);
        // Send message to message queue
        msg_send(&msg, msg_queue);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Create the timer thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, timer_thread, NULL, "timer");
    
    return 0;
}