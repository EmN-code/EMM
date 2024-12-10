
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>
#include <string.h>
#include <time.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE_SEC 60
#define MQTT_CLEAN_SESSION 1

struct mqtt_sn_client client;
uint8_t rx_buffer[128];
uint8_t tx_buffer[128];

void mqtt_event_handler(struct mqtt_sn_client *client, struct mqtt_sn_evt *evt)
{
    switch (evt->type) {
        case MQTT_SN_EVT_CONNECTED:
            printk("MQTT-SN connected\n");
            break;
        case MQTT_SN_EVT_DISCONNECTED:
            printk("MQTT-SN disconnected\n");
            break;
        case MQTT_SN_EVT_PUBLISH:
            printk("MQTT-SN publish received\n");
            break;
        default:
            break;
    }
}

void publish_state(struct mqtt_sn_client *client)
{
    struct mqtt_sn_publish_param param;
    struct mqtt_sn_topic topic;
    struct mqtt_sn_binstr payload;

    topic.name = MQTT_TOPIC;
    topic.name_len = strlen(MQTT_TOPIC);

    payload.data = (uint8_t *)MQTT_PAYLOAD;
    payload.data_len = MQTT_PAYLOAD_SIZE;

    param.topic = &topic;
    param.payload = &payload;
    param.qos = MQTT_SN_QOS_0;
    param.retain = 0;

    mqtt_sn_publish(client, &param);
}

int main(void)
{
    struct sockaddr_in broker_addr;
    int sock;

    memset(&client, 0, sizeof(client));
    client.client_id.utf8_str = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.keepalive = MQTT_KEEPALIVE_SEC;
    client.clean_session = MQTT_CLEAN_SESSION;
    client.evt_cb = mqtt_event_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to broker\n");
        close(sock);
        return -1;
    }

    client.transport.type = MQTT_SN_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    if (mqtt_sn_connect(&client, false, true) < 0) {
        printk("Failed to connect to MQTT-SN broker\n");
        close(sock);
        return -1;
    }

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    mqtt_sn_disconnect(&client);
    close(sock);

    return 0;
}
