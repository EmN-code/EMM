
#include "thread.h"
#include "irq.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static kernel_pid_t pid;

static void *thread_func(void *arg)
{
    (void)arg;
    while (1) {
        xtimer_sleep(1);
        printf("Thread running\n");
    }
    return NULL;
}

static void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt handled\n");
}

int main(void)
{
    // Create a thread
    pid = thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread");

    // Set an interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable interrupts
    irq_enable(0);

    // Main loop
    while (1) {
        xtimer_sleep(2);
        printf("Main loop running\n");
    }

    return 0;
}
