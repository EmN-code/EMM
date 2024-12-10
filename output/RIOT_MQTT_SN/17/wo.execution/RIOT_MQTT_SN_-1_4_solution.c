// Import necessary header files
#include "ztimer.h" // For sleep functionality
#include "unistd.h" // For POSIX standard functions
#include "stdlib.h" // For standard library functions
#include "stdio.h" // For standard I/O functions
#include "kernel_defines.h" // For RIOT kernel definitions
#include "mutex.h" // For mutex operations
#include "mma8x5x.h" // For specific sensor header
#include "mma8x5x_params.h" // For sensor parameters header
#include "mma8x5x_regs.h" // For sensor registers header
#include "mbox.h" // For mailbox operations
#include "evtimer_mbox.h" // For event timer with mailbox
#include "evtimer_msg.h" // For event timer message
#include "timex.h" // For time extensions
#include "time_units.h" // For time unit definitions
#include "arpa/inet.h" // For internet operations (e.g., address translation)
#include "netinet/in.h" // For internet addresses
#include "sys/socket.h" // For socket operations
#include "net/sock/udp.h" // For UDP socket operations
#include "net/sock/util.h" // For socket utility functions
#include "net/emcute.h" // For emcute protocol functions
#include "net/gcoap.h" // For CoAP functions (optional for this task)

// Define the address of the MQTT-SN broker
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"

static emcute_topic_t topic;
static sock_udp_ep_t gw;

void mqtt_publish(const char *data) {
    // Publish data to the topic
    if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: Unable to publish data.\n");
    }
    else {
        printf("Published: %s\n", data);
    }
}

int main(void) {
    // Initialize the MQTT-SN gateway address
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    if (sock_addr_in_init(&gw.addr, inet_addr(BROKER_IP), gw.port) < 0) {
        printf("Error: Unable to initialize gateway address.\n");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Unable to connect to the MQTT-SN gateway.\n");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: Unable to register topic.\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish("work");
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5000 ms
    }

    return 0;
}