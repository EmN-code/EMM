#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mbox.h"
#include "msg.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "time_units.h"
#include "arpa/inet.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED0_PIN      GPIO_PIN(PORT_A, 5)  // Adjust pin as required

event_t timer_event;
(kernel_pid_t) recvid(){
    msg_t msg;
    while(1) {
        msg_receive(&msg);
    }
}

void cb(void *arg){
    (void)arg;
    gpio_toggle(LED0_PIN); 
}

int main(void) {
    msg_init_queue(msgQ, 100);

    gpio_init(LED0_PIN, GPIO_OUT);

    printf("This will toggle the LED every second\n");

    do {
        msg_t ignore;
        { if (!(ztimer_msec_based_cb(&timer_event, TIMER_INTERVAL, &cb))) {
            continue;
        }
        msg_send(&ignore, recvid());
    }} while (1);

    return 0;
}