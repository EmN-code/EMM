
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static kernel_pid_t pid;

static atomic_int_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_store(&atomic_var, 1);
}

void test_thread(void)
{
    while (1) {
        if (atomic_load(&atomic_var) == 1) {
            printf("Interrupt handled correctly!\n");
            atomic_store(&atomic_var, 0);
        }
        thread_yield();
    }
}

int main(void)
{
    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, test_thread, NULL, "test_thread");

    irq_set_handler(0, interrupt_handler, NULL);

    printf("Starting interrupt test...\n");

    return 0;
}
