#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <timex.h>
#include <thread.h>
#include <time_units.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>

void alarm_handler(void)
{
    printf("Alarm triggered!\n");
}

int setRTCAlarm(void)
{
    // This function simulates setting an RTC alarm after 5 seconds
    printf("Setting RTC time to June 1, 2024, 00:00:00.\n");
    // Simulating time and alarm, sleep for 5 seconds to represent elapsed time
    xtimer_sleep(5);
    alarm_handler();
    return 0;
}

int main(void)
{
    printf("Initializing RTC Alarm\n");
    while(setRTCAlarm() != 0) {
        printf("Retry setting RTC alarm.\n");
        xtimer_sleep(1);
    }
    return 0;
}