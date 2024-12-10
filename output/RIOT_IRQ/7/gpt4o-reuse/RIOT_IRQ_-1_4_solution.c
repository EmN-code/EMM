
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <irq.h>

#define THREAD_STACKSIZE_DEFAULT (512)

static char thread_stack[THREAD_STACKSIZE_DEFAULT];
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

    // Create a thread to monitor the atomic variable
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_func, NULL, "monitor_thread");

    // Set the interrupt handler
    irq_set_handler(11, irq_handler);

    // Enable the interrupt
    irq_enable(11);

    // Main loop to keep the program running
    while (1) {
        xtimer_sleep(5);
    }

    return 0;
}
