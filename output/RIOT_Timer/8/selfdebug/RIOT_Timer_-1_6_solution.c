#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC) // Timer interval: 1 second

static kernel_pid_t main_pid;
static char stack[THREAD_STACKSIZE_MAIN];
static xtimer_t t;

void timer_loop(void)
{
    while(1) {
        xtimer_usleep(TIMER_INTERVAL);
        gpio_set(GPIO_PIN(PORT_A, 5)); // Assuming the LED is connected to Pin A5
    }
}

int main(void)
{
    // Initialize the LED pin
    gpio_init(GPIO_PIN(PORT_A, 5), GPIO_OUT);
    
    // Start the timer loop
    main_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, timer_loop, NULL, "timer_loop");
    thread_yield();

    return 0;
}