
#include "irq.h"
#include "event/thread.h"
#include "thread.h"

#define ATOMIC_INIT(val) { .counter = (val) }

typedef struct {
    volatile int counter;
} atomic_t;

static atomic_t atomic_var = ATOMIC_INIT(0);

static void atomic_store(atomic_t *v, int i)
{
    v->counter = i;
}

static int atomic_load(atomic_t *v)
{
    return v->counter;
}

static void my_event_handler(event_t *event)
{
    (void)event;
    unsigned state = irq_disable();
    atomic_store(&atomic_var, 1);
    irq_restore(state);
}

int main(void)
{
    event_queue_t event_queue;
    event_t event;

    event_queue_init(&event_queue);

    unsigned state = irq_disable();
    atomic_store(&atomic_var, 0);
    irq_restore(state);

    event_post(&event_queue, &event);

    thread_sleep();

    state = irq_disable();
    int value = atomic_load(&atomic_var);
    irq_restore(state);

    if (value == 1) {
        printf("Interrupts were disabled and restored correctly.\n");
    } else {
        printf("Interrupts were not handled correctly.\n");
    }

    return 0;
}
