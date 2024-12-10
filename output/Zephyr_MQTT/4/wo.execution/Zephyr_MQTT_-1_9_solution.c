
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/posix/arpa/inet.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work or not work"

struct mqtt_sn_client client;
struct mqtt_sn_data client_id;
struct mqtt_sn_data topic_name;
struct mqtt_sn_data message_data;
struct mqtt_sn_transport transport;

void mqtt_sn_event_handler(struct mqtt_sn_client *client, enum mqtt_sn_evt_type evt, void *data)
{
    if (evt == MQTT_SN_EVT_CONNECTED) {
        printk("MQTT-SN connected to broker\n");
    } else if (evt == MQTT_SN_EVT_DISCONNECTED) {
        printk("MQTT-SN disconnected from broker\n");
    }
}

int main(void)
{
    int sock;
    struct sockaddr_in broker_addr;

    // Initialize client ID
    client_id.data = MQTT_CLIENT_ID;
    client_id.size = strlen(MQTT_CLIENT_ID);

    // Initialize topic name
    topic_name.data = MQTT_TOPIC;
    topic_name.size = strlen(MQTT_TOPIC);

    // Initialize message data
    message_data.data = MQTT_MESSAGE;
    message_data.size = strlen(MQTT_MESSAGE);

    // Initialize transport
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to broker\n");
        close(sock);
        return -1;
    }

    transport.tcp.sock = sock;

    // Initialize MQTT-SN client
    mqtt_sn_client_init(&client, &client_id, &transport, mqtt_sn_event_handler, NULL, 0, NULL, 0);

    // Connect to MQTT-SN broker
    if (mqtt_sn_connect(&client, false, true) != 0) {
        printk("Failed to connect to MQTT-SN broker\n");
        close(sock);
        return -1;
    }

    while (1) {
        // Publish message
        if (mqtt_sn_publish(&client, MQTT_SN_QOS_0, &topic_name, false, &message_data) != 0) {
            printk("Failed to publish message\n");
        }

        k_sleep(K_SECONDS(5));
    }

    // Disconnect from MQTT-SN broker
    mqtt_sn_disconnect(&client);
    close(sock);

    return 0;
}
