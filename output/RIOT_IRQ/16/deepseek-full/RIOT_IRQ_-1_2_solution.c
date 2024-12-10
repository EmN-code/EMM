
#include "event/thread.h"
#include "irq.h"
#include "thread.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static volatile int interrupt_flag = 0;
static char stack[THREAD_STACKSIZE];

static void *interrupt_handler(void *arg)
{
    (void)arg;
    interrupt_flag = 1;
    return NULL;
}

int main(void)
{
    unsigned state = irq_disable();

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, interrupt_handler, NULL, "interrupt_handler");

    irq_restore(state);

    // Simulate an interrupt trigger
    thread_yield();

    if (interrupt_flag) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt handling failed.");
    }

    return 0;
}
