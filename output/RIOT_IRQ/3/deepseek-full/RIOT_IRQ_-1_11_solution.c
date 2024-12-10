
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "irq.h"
#include "stdlib.h"
#include "ztimer.h"
#include "stddef.h"
#include "net/sock/util.h"
#include "mutex.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "timex.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "stdio.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "mma8x5x.h"

#define MSG_QUEUE_SIZE 16

static msg_t msg_queue[MSG_QUEUE_SIZE];
static volatile atomic_int atomic_var = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    unsigned state = irq_disable();
    irq_set_handler(UART_IRQ_NUM, interrupt_handler, NULL);
    irq_restore(state);

    while (1) {
        if (atomic_load(&atomic_var) > 0) {
            printf("Interrupt handled, atomic variable incremented.\n");
            atomic_store(&atomic_var, 0);
        }
        xtimer_sleep(1);
    }

    return 0;
}
