#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "sys/socket.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "struct_mgmt.h"
#include "stdlib.h"
#include "kernel_defines.h"
#include "msg.h"
#include "mutex.h"
#include "evtimer_msg.h"
#include "architecture.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "unistd.h"

#define ADDR "192.168.1.1"
#define PORT 1883

static int connect_network(void) {
    // Simulate the network connection establishment
    while (1) {
        int rc = NetworkConnect(NULL, ADDR, PORT);  // Passing NULL because we are simulating
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }
        printf("Network connected successfully\n");
        break;
    }
    return 0;
}

static int read_accelerometer_data(void) {
    // Simulate reading accelerometer data
    printf("Reading accelerometer data\n");
    return 0;
}

int main(void) {
    // Simulate delay and accelerometer reading every 5 seconds
    while (1) {
        read_accelerometer_data();
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}
