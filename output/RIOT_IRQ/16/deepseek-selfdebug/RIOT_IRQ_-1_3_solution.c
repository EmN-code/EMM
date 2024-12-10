
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static kernel_pid_t pid;
static volatile int atomic_var = 0;

void interrupt_handler(void)
{
    atomic_var = 1;
}

void worker_thread(void)
{
    while (1) {
        if (atomic_var == 1) {
            printf("Interrupt handled, atomic_var set to 1\n");
            atomic_var = 0;
        }
        xtimer_usleep(100000); // Sleep for 100ms
    }
}

int main(void)
{
    // Initialize the worker thread
    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, worker_thread, NULL, "worker");

    // Enable interrupts and set the handler
    irq_enable();
    irq_set_handler(0, interrupt_handler);

    // Main loop
    while (1) {
        xtimer_usleep(1000000); // Sleep for 1s
    }

    return 0;
}
