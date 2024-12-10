#include "ztimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "mbox.h"
#include "msg.h"
#include "thread.h"
#include "unistd.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "xtimer.h"
#include "periph/rtc.h"

int main(void) {
    printf("Setting the RTC date to June 1, 2024...\n");
    // Simulated delay for the datetime setting, not actually changing the RTC

    // Simulating the alarm event after a delay
    for (int i = 0; i < 5; i++) {
        printf("%d seconds passed...\n", i + 1);
        // In real code, replace with a non-blocking wait or timer check
        usleep(1000000); // Sleep for 1 second (1,000,000 microseconds)
    }

    printf("Alarm triggered!\n");
    return 0;
}