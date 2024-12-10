#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Example LED pin
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

static kernel_pid_t main_thread_pid;

static void *led_toggle_handler(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        // Toggle the LED
        gpio_toggle(LED_PIN);
        printf("LED toggled!\n");
    }
    return NULL;
}

int main(void) {
    // Initialize GPIO pin for LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Start the LED toggle thread
    main_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                                    THREAD_CREATE_STACKTEST, led_toggle_handler,
                                    NULL, "led_toggle");

    // Setup timer message and timer
    static msg_t timer_msg;
    static xtimer_t timer;
    timer.target = main_thread_pid;
    timer.msg = &timer_msg;

    while (1) {
        xtimer_set_msg(&timer, TIMER_INTERVAL, &timer_msg, main_thread_pid);
        // The timer will send the message every second to the handler
    }

    return 0;
}

static char stack[THREAD_STACKSIZE_MAIN];