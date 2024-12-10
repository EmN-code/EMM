
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static struct sockaddr_in broker_addr;

void connect_to_broker(void) {
    int res;

    // Initialize the socket
    res = sock_tcp_create(&sock, NULL, NULL, SOCK_ADDR_ANY_NETIF, 0);
    if (res < 0) {
        puts("Error creating socket");
        return;
    }

    // Set up the broker address
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    // Connect to the broker
    res = sock_tcp_connect(&sock, (struct sockaddr *)&broker_addr, 0);
    if (res < 0) {
        puts("Error connecting to broker");
        sock_tcp_close(&sock);
        return;
    }

    puts("Connected to MQTT broker");
}

void publish_message(void) {
    int res;
    mqtt_publish_t pub;
    mqtt_message_t msg;

    // Prepare the publish message
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_MESSAGE;
    pub.payload_len = strlen(MQTT_MESSAGE);
    pub.qos = MQTT_QOS_0;
    pub.retain = 0;

    // Prepare the MQTT message
    msg.type = MQTT_MSG_TYPE_PUBLISH;
    msg.dup = 0;
    msg.qos = MQTT_QOS_0;
    msg.retain = 0;
    msg.length = mqtt_get_publish_msg_len(&pub);
    msg.payload = (uint8_t *)&pub;

    // Send the MQTT message
    res = mqtt_send(&sock, &msg);
    if (res < 0) {
        puts("Error publishing message");
    } else {
        puts("Message published");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    connect_to_broker();

    while (1) {
        publish_message();
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    return 0;
}
