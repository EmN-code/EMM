#include <stdio.h>
#include <string.h>

#include "mqttsn_publisher.h"
#include "net/mqttsn.h"
#include "net/sock/udp.h"
#include "net/sock.h"
#include "net/emcute.h"
#include "msg.h"

#define SERVER_ADDR_IPV4 "47.102.103.1"
#define SERVER_PORT (1883)
#define CLIENT_ID "esp32_client"
#define BUFFER_SIZE 128
#define KEEP_ALIVE 10

static char buffer[BUFFER_SIZE];
msg_t msg_q[8];

static void on_pub(uint8_t* topic, void* data, size_t len) {
    // Callback if needed
}

int main(void) {
    puts("MQTT Publisher App Started");

    /* Initialize the message queue */
    msg_init_queue(msg_q, 8);

    mqttsn_publisher_t pub;
    mqttsn_pub_t msg_pub;

    /* Preparing MQTT-SN UDP socket */
    sock_udp_ep_t broker_ep = { .family = AF_INET, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&broker_ep.addr, SERVER_ADDR_IPV4);

    /* Initialize MQTT-SN publisher data structure */
    mqttsn_publisher_init(&pub, buffer, sizeof(buffer), CLIENT_ID, KEEP_ALIVE, msg_q);

    while (1) {
        /* Attempt to connect, if not success retry */
        if (mqttsn_publisher_connect(&pub, &broker_ep) != MQTT_OK) {
            puts("Connection failed, retrying...");
            xtimer_sleep(5);
            continue;
        }

        /* Prepare the message to publish */
        msg_pub.topic = "device/state";
        msg_pub.data = "work"; // or "not work"
        msg_pub.data_len = strlen(msg_pub.data);

        while (1) {
            /* Publish message every 5 seconds */
            mqttsn_publisher_publish(&pub, &msg_pub);
            xtimer_sleep(5);
        }

        mqttsn_publisher_disconnect(&pub);
    }

    return 0;
}