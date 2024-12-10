
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/unistd.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_NOT "not work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_MESSAGE_ID 1
#define MQTT_RX_BUF_SIZE 1024
#define MQTT_TX_BUF_SIZE 1024
#define MQTT_PUBLISH_INTERVAL 5000

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[MQTT_RX_BUF_SIZE];
static uint8_t tx_buffer[MQTT_TX_BUF_SIZE];

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

void mqtt_publish_state(struct mqtt_client *client, const char *state)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
        .qos = MQTT_QOS
    };

    param.message.topic = topic;
    param.message.payload.data = state;
    param.message.payload.len = strlen(state);
    param.message_id = MQTT_MESSAGE_ID;
    param.dup_flag = MQTT_DUP;
    param.retain_flag = MQTT_RETAIN;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        printk("Failed to publish message: %d\n", ret);
    } else {
        printk("Published: %s\n", state);
    }
}

void main(void)
{
    struct sockaddr_in broker;
    int sock;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", sock);
        return;
    }

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return;
    }

    mqtt_client_init(&client_ctx);
    client_ctx.broker = &broker;
    client_ctx.client_id.utf8 = MQTT_CLIENT_ID;
    client_ctx.client_id.size = strlen(MQTT_CLIENT_ID);
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.transport.tcp.sock = sock;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = MQTT_RX_BUF_SIZE;
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = MQTT_TX_BUF_SIZE;
    client_ctx.keepalive = MQTT_KEEPALIVE;
    client_ctx.clean_session = MQTT_CLEAN_SESSION;

    int ret = mqtt_connect(&client_ctx);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        close(sock);
        return;
    }

    while (1) {
        mqtt_publish_state(&client_ctx, MQTT_PAYLOAD);
        k_sleep(K_SECONDS(MQTT_PUBLISH_INTERVAL / 1000));
        mqtt_publish_state(&client_ctx, MQTT_PAYLOAD_NOT);
        k_sleep(K_SECONDS(MQTT_PUBLISH_INTERVAL / 1000));
    }
}
