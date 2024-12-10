
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <irq.h>
#include <atomic_utils.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t pid;
static atomic_int flag = ATOMIC_INIT(0);

void *thread_func(void *arg)
{
    (void)arg;

    while (1) {
        if (atomic_load(&flag)) {
            printf("Interrupt occurred and flag is set.\n");
            atomic_store(&flag, 0);
        }
        xtimer_sleep(1);
    }

    return NULL;
}

void irq_handler(void)
{
    atomic_store(&flag, 1);
    printf("Interrupt handler triggered.\n");
}

int main(void)
{
    pid = thread_create(NULL, THREAD_STACKSIZE, THREAD_PRIORITY, 0, thread_func, NULL, "irq_test");

    printf("Starting interrupt test...\n");

    // Disable interrupts
    irq_disable();

    // Set the interrupt handler
    irq_set_handler(0, irq_handler);

    // Enable interrupts
    irq_enable();

    // Simulate an interrupt
    irq_handler();

    // Disable interrupts again
    irq_disable();

    // Enable interrupts again
    irq_enable();

    printf("Interrupt test completed.\n");

    return 0;
}
