
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/dns_resolve.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct mqtt_topic topic;

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("Connected to MQTT broker\n");
            } else {
                printk("Failed to connect to MQTT broker: %d\n", evt->result);
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        default:
            break;
    }
}

int main(void)
{
    int sock;
    struct sockaddr_in broker_addr;
    struct mqtt_publish_param param;
    struct mqtt_publish_message msg;

    // Initialize MQTT client
    memset(&client, 0, sizeof(client));
    client.broker = &broker_addr;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen(client.client_id.utf8);
    client.keepalive = 60;
    client.clean_session = 1;

    // Initialize MQTT topic
    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    // Initialize MQTT message
    msg.topic = &topic;
    msg.payload.data = MQTT_PAYLOAD;
    msg.payload.len = strlen(MQTT_PAYLOAD);

    // Initialize MQTT publish parameters
    param.message = msg;
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set broker address
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    // Connect to MQTT broker
    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    // Set socket in MQTT client
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    // Connect MQTT client
    if (mqtt_connect(&client) != 0) {
        printk("Failed to connect MQTT client\n");
        close(sock);
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        if (mqtt_publish(&client, &param) != 0) {
            printk("Failed to publish message\n");
        } else {
            printk("Message published\n");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
