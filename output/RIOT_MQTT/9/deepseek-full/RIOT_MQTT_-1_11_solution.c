
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/sock/tcp.h>
#include <xtimer.h>
#include <msg.h>
#include <evtimer_msg.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static evtimer_msg_t evtimer;
static sock_tcp_t sock;

void publish_state(void) {
    uint8_t buffer[100];
    int len = snprintf((char *)buffer, sizeof(buffer), "PUBLISH %s %s\n", MQTT_TOPIC, MQTT_PAYLOAD);

    if (sock_tcp_send(&sock, buffer, len, 0) < 0) {
        puts("Failed to send message");
    } else {
        puts("Message sent successfully");
    }
}

void _evtimer_msg_handler(void *arg) {
    (void)arg;
    publish_state();
}

int main(void) {
    sock_ipaddr_t broker_addr;
    msg_t msg;

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.u8) <= 0) {
        puts("Invalid IP address");
        return 1;
    }

    // Create TCP socket
    if (sock_tcp_create(&sock, NULL, &broker_addr, MQTT_BROKER_PORT, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Failed to create socket");
        return 1;
    }

    // Connect to the MQTT broker
    if (sock_tcp_connect(&sock, &broker_addr, MQTT_BROKER_PORT, 0) < 0) {
        puts("Failed to connect to broker");
        sock_tcp_close(&sock);
        return 1;
    }

    puts("Connected to MQTT broker");

    // Schedule the first event
    evtimer_add_msg(&evtimer, (evtimer_msg_event_t *)&msg, thread_getpid());

    while (1) {
        msg_receive(&msg);
        xtimer_usleep(INTERVAL);
        evtimer_add_msg(&evtimer, (evtimer_msg_event_t *)&msg, thread_getpid());
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
