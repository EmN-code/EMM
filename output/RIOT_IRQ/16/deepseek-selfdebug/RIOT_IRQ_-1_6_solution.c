
#include "irq.h"
#include "thread.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include "msg.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static atomic_int_t atomic_var = ATOMIC_INIT(0);

void *thread_func(void *arg)
{
    (void)arg;

    while (1) {
        xtimer_sleep(1);
        atomic_store(&atomic_var, 1);
    }

    return NULL;
}

void irq_handler(void *arg)
{
    (void)arg;

    atomic_store(&atomic_var, 0);
}

int main(void)
{
    kernel_pid_t pid;

    pid = thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread");

    irq_set_handler(0, irq_handler, NULL);

    while (1) {
        irq_disable();
        int value = atomic_load(&atomic_var);
        irq_enable();

        if (value == 0) {
            printf("Interrupt handled correctly, atomic variable is 0\n");
        } else {
            printf("Interrupt handling failed, atomic variable is %d\n", value);
        }

        xtimer_sleep(2);
    }

    return 0;
}
