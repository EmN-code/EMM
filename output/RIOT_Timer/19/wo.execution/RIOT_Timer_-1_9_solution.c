#include "xtimer.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include "thread.h"
#include "msg.h"

// LED pin definition (For ESP32, GPIO number should be used)
#define LED_PIN GPIO_PIN(PORT_A, 2)

// Thread stack for the LED control thread
char led_thread_stack[THREAD_STACKSIZE_DEFAULT];

// Function to toggle the LED state
void *led_control_thread(void *arg) {
    (void)arg;
    msg_t msg;
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        msg_receive(&msg); // Wait for a message to toggle LED
        gpio_toggle(LED_PIN); // Toggle LED
    }

    return NULL;
}

int main(void) {
    msg_t msg;
    msg_t msg_queue[8];

    // Initialize message queue for the LED thread
    msg_init_queue(msg_queue, 8);

    // Create the LED control thread
    thread_create(led_thread_stack, sizeof(led_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, led_control_thread, NULL, "led_control");

    while (1) {
        xtimer_sleep(1); // Sleep for 1 second
        msg_send(&msg, thread_getpid() + 1); // Send message to LED control thread to toggle LED
    }

    return 0;
}