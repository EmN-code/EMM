
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <irq.h>
#include <atomic_utils.h>

#define THREAD_STACKSIZE_DEFAULT (512)

static kernel_pid_t irq_thread_pid;
static char irq_thread_stack[THREAD_STACKSIZE_DEFAULT];
static atomic_t irq_flag = ATOMIC_INIT(0);

static void *irq_thread(void *arg)
{
    (void)arg;

    while (1) {
        if (atomic_load(&irq_flag)) {
            printf("Interrupt handled!\n");
            atomic_store(&irq_flag, 0);
        }
        xtimer_sleep(1);
    }

    return NULL;
}

static void irq_handler(void)
{
    atomic_store(&irq_flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Create the IRQ handling thread
    irq_thread_pid = thread_create(irq_thread_stack, sizeof(irq_thread_stack),
                                   THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                   irq_thread, NULL, "irq_thread");

    // Set the IRQ handler
    irq_set(0, irq_handler);

    // Disable and then enable the IRQ to verify functionality
    irq_disable(0);
    xtimer_sleep(2);
    irq_enable(0);

    // Trigger the IRQ manually for testing
    irq_handler();

    return 0;
}
