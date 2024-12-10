
#include <stdio.h>
#include <xtimer.h>
#include <irq.h>
#include <thread.h>

#define IRQ_NUM 11

static volatile int atomic_var = 0;

static void *irq_handler_thread(void *arg)
{
    (void)arg;
    while (1) {
        if (atomic_var == 1) {
            printf("Interrupt handled!\n");
            atomic_var = 0;
        }
        xtimer_usleep(100000); // Sleep for 100ms
    }
    return NULL;
}

static void irq_handler(void)
{
    atomic_var = 1;
}

int main(void)
{
    kernel_pid_t pid;

    // Initialize the interrupt handler thread
    pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, irq_handler_thread, NULL, "irq_handler");

    // Register the interrupt handler
    irq_disable(IRQ_NUM);
    irq_set_handler(IRQ_NUM, irq_handler);
    irq_enable(IRQ_NUM);

    // Simulate an interrupt
    printf("Simulating an interrupt...\n");
    irq_handler();

    // Disable the interrupt
    irq_disable(IRQ_NUM);
    printf("Interrupt disabled.\n");

    // Re-enable the interrupt
    irq_enable(IRQ_NUM);
    printf("Interrupt re-enabled.\n");

    // Simulate another interrupt
    printf("Simulating another interrupt...\n");
    irq_handler();

    return 0;
}
