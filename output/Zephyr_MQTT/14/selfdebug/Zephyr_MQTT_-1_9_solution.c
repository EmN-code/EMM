
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>
#include <string.h>
#include <errno.h>
#include <net/mqtt.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(main);

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const c,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("Connection error: %d", evt->result);
        } else {
            LOG_INF("Connected to broker");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("Disconnected from broker");
        break;
    default:
        break;
    }
}

void connect_to_broker(void)
{
    int rc;
    struct mqtt_connect_param conn_param;

    memset(&conn_param, 0, sizeof(conn_param));
    conn_param.client_id.utf8 = (uint8_t *)"zephyr_client";
    conn_param.client_id.size = strlen("zephyr_client");
    conn_param.keepalive = 60;
    conn_param.clean_session = 1;

    rc = mqtt_connect(&client, &conn_param);
    if (rc) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", rc);
    }
}

void publish_state(void)
{
    int rc;
    struct mqtt_publish_param param;

    param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = (uint8_t *)MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.message_id = 1U;
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    rc = mqtt_publish(&client, &param);
    if (rc) {
        LOG_ERR("Failed to publish message, error: %d", rc);
    } else {
        LOG_INF("Message published");
    }
}

void main(void)
{
    int rc;
    struct sockaddr_in broker_addr;

    memset(&client, 0, sizeof(client));
    client.broker = &broker_addr;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    rc = mqtt_client_init(&client);
    if (rc) {
        LOG_ERR("Failed to initialize MQTT client, error: %d", rc);
        return;
    }

    connect_to_broker();

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state();
    }
}
