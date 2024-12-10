#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "msg.h"
#include "event/thread.h"

#define STACKSIZE        (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY  (THREAD_PRIORITY_MAIN - 1)
#define GPIO_PIN         GPIO_PIN(PORT_A, 2)

void *dht_read_thread(void *arg) {
    (void)arg;
    while (1) {
        int16_t temp, hum;
        if (dht_read(NULL, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid;
    char stack[STACKSIZE];

    gpio_init(GPIO_PIN, GPIO_OUT);

    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0, dht_read_thread, NULL, "dht_reader");
    if (pid <= 0) {
        printf("Error: Could not create thread\n");
        return 1;
    }

    while (1) {
        
    }

    return 0;
}