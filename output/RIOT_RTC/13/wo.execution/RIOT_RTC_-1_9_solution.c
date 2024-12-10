#include "ztimer.h"
#include "netinet/in.h"
#include "timex.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stddef.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "mutex.h"
#include "timex.h"
#include "time_units.h"
#include "esp_system.h"

#define ALARM_OFFSET 5

void alarm_cb(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_msg_event_t evtimer_event;
    msg_t msg;

    msg_receive(&msg);

    evtimer_init_msg(&evtimer);

    ztimer_set_timeout(ZTIMER_SEC, ztimer_now(ZTIMER_SEC) + ALARM_OFFSET);
    msg.content.value = ALARM_OFFSET;
    msg_send_receive(&msg, NULL, evtimer_event.pid);

    alarm_cb(NULL);
    
    return 0;
}