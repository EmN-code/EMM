
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"

#define STACK_SIZE 512
#define PRIORITY   THREAD_PRIORITY_MAIN - 1

static char stack[STACK_SIZE];
static volatile bool interrupt_occurred = false;

void interrupt_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = true;
}

void *thread_func(void *arg)
{
    (void)arg;
    while (1) {
        if (interrupt_occurred) {
            printf("Interrupt occurred!\n");
            interrupt_occurred = false;
        }
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, 0, thread_func, NULL, "interrupt_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    unsigned state = irq_disable();
    irq_set_handler(ESP32_IRQ_NUM, interrupt_handler);
    irq_restore(state);

    irq_enable(ESP32_IRQ_NUM);

    while (1) {
        // Main loop
    }

    return 0;
}
