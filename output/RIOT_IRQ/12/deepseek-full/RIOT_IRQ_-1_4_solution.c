
#include "evtimer_msg.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "stdlib.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "stdatomic.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "event/thread.h"
#include "stdbool.h"
#include "MQTTClient.h"
#include "irq.h"
#include "msg.h"
#include "thread.h"
#include "mbox.h"
#include "stddef.h"
#include "ztimer.h"

atomic_int isr_flag = ATOMIC_VAR_INIT(0);

void isr_handler(void *arg)
{
    (void)arg;
    atomic_store(&isr_flag, 1);
}

int main(void)
{
    unsigned state = irq_disable();

    // Simulate an interrupt
    isr_handler(NULL);

    // Restore interrupts
    irq_restore(state);

    // Verify if the interrupt was handled correctly
    if (atomic_load(&isr_flag) == 1) {
        printf("Interrupt handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
