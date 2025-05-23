#include <stdint.h>
#include <stdatomic.h>
#include <stdio.h>

#include "fmt.h"
#include "irq.h"
#include "xtimer.h"

#define DELAY       (10 * US_PER_MS)

static atomic_uint a = ATOMIC_VAR_INIT(0);
static atomic_uint a_during_isr = ATOMIC_VAR_INIT(0);
static atomic_uint b = ATOMIC_VAR_INIT(0);
static atomic_uint b_during_isr = ATOMIC_VAR_INIT(0);

static void busy_delay(void)
{
    uint32_t start = xtimer_now_usec();
    while (xtimer_now_usec() - start < DELAY) { }
}

static void timer_callback(void *unused)
{
    (void)unused;
    atomic_store(&a_during_isr, atomic_load(&a));
    atomic_store(&b_during_isr, atomic_load(&b));
}

int main(void)
{
    xtimer_t xt = { .callback = timer_callback };

    print_str("Test for irq_disable() / irq_restore()\n"
              "======================================\n"
              "\n");

    print_str("Verifying IRQ state tracking works: ");
    int state_a = irq_is_enabled();
    unsigned state = irq_disable();
    int state_b = irq_is_enabled();
    irq_restore(state);
    int state_c = irq_is_enabled();
    if ((state_a != 0) && (state_b == 0) && (state_c != 0)) {
        print_str("[SUCCESS]\n");
    }
    else {
        print_str("[FAILURE]\n");
    }

    print_str("Verifying test works: ");
    xtimer_set(&xt, DELAY / 2);
    atomic_store(&a, 1);
    busy_delay();
    atomic_store(&b, 1);

    if ((atomic_load(&a) == atomic_load(&a_during_isr)) &&
            (atomic_load(&b) != atomic_load(&b_during_isr)))
        {
        print_str("[SUCCESS]\n");
    }
    else {
        print_str("[FAILURE]\n");
    }

    print_str("Test result: ");
    xtimer_set(&xt, DELAY / 2);
    state = irq_disable();
    atomic_store(&a, 2);
    busy_delay();
    atomic_store(&b, 2);
    irq_restore(state);

    if ((atomic_load(&a) == atomic_load(&a_during_isr)) &&
            (atomic_load(&b) == atomic_load(&b_during_isr)))
        {
        print_str("[SUCCESS]\n");
    }
    else {
        print_str("[FAILURE]\n");
    }

    return 0;
}
