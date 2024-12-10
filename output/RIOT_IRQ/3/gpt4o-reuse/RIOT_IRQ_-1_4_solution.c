
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <irq.h>

#define THREAD_STACKSIZE_DEFAULT (512)
#define DEBUG_EXTRA_STACKSIZE (0)

static char thread_stack[THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE];
static kernel_pid_t thread_pid;
static volatile int atomic_var = 0;

static void *thread_func(void *arg)
{
    (void)arg;

    while (1) {
        xtimer_sleep(1);
        printf("Thread running, atomic_var: %d\n", atomic_var);
    }

    return NULL;
}

static void irq_handler(void)
{
    unsigned state = irq_disable();
    atomic_var++;
    irq_restore(state);
    printf("Interrupt handled, atomic_var incremented.\n");
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the thread
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_func, NULL, "interrupt_test_thread");

    // Set the IRQ handler
    irq_set(11, irq_handler);

    // Enable the IRQ
    irq_enable(11);

    // Main loop
    while (1) {
        xtimer_sleep(2);
        printf("Main loop running...\n");
    }

    return 0;
}
