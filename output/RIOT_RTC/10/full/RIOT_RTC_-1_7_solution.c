#include "sys/socket.h"  // Import socket functions 
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "paho_mqtt.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "ztimer.h"
#include "timex.h"
#include "xtimer.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "pthread.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "periph/rtc.h"
#include "mutex.h"

#define I2C_ADDR      (0x1E) // Example I2C address, change as needed

void setup_rtc_and_alarm(void) {
    // Assuming the RTC is connected over I2C, with some fixed address I2C_ADDR
    uint8_t buf[7];

    // Set date to June 1, 2024
    buf[0] = 0x00; // Assume register 0x00 is for seconds, etc.
    buf[1] = 0x00; // Minute 0
    buf[2] = 0x12; // Hour 12:00
    buf[3] = 0x01; // Day 1
    buf[4] = 0x06; // Month June
    buf[5] = 0x24; // Year 2024
    i2c_write(I2C_DEV(0), I2C_ADDR, buf, sizeof(buf), 0);

    // Set alarm after 5 seconds
    buf[0] = 0x05; // Set seconds for alarm
    i2c_write(I2C_DEV(0), I2C_ADDR, buf, 1, 0);
}

void *alarm_handler(void *arg) {
    while (1) {
        // Simulate waiting for the alarm
        sleep(5); // Wait 5 seconds to simulate alarm trigger
        printf("Alarm triggered!\n");
        break; // Exit after one alarm
    }
    return NULL;
}

int main(void) {
    pthread_t alarm_thread;

    setup_rtc_and_alarm();

    // Start a thread to handle the alarm
    pthread_create(&alarm_thread, NULL, alarm_handler, NULL);
    pthread_join(alarm_thread, NULL);

    return 0;
}