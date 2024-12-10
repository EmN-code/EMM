#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_mgmt.h>

LOG_MODULE_REGISTER(mqtt_simple, LOG_LEVEL_INF);

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_TOPIC "esp32/state"
#define PAYLOAD "work or not work"
#define APP_SLEEP_MSECS 5000

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct zsock_pollfd fds;
static int nfds;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("Could not connect to broker (%d)", evt->result);
            return;
        }
        LOG_INF("Connected to broker");
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("Disconnected from broker");
        break;
    case MQTT_EVT_PUBLISH:
        LOG_INF("Received publish");
        break;
    default:
        break;
    }
}

void broker_init(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(SERVER_PORT);
    zsock_inet_pton(AF_INET, SERVER_ADDR, &broker4->sin_addr);
}

void client_init(void)
{
    mqtt_client_init(&client);

    broker_init();

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"esp32_id";
    client.client_id.size = strlen("esp32_id");
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    struct mqtt_sec_config *tls_config = &client.transport.tls.config;
    memset(tls_config, 0, sizeof(struct mqtt_sec_config));

    nfds = 1;
    fds.sock = client.transport.tcp.sock;
    fds.events = ZSOCK_POLLIN | ZSOCK_POLLOUT;
}

int publish(struct mqtt_client *c)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = PUB_TOPIC;
    param.message.topic.topic.size = strlen(PUB_TOPIC);
    param.message.payload.data = PAYLOAD;
    param.message.payload.len = strlen(PAYLOAD);
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    return mqtt_publish(c, &param);
}

void main(void)
{
    LOG_INF("Starting MQTT sample");

    client_init();

    if (mqtt_connect(&client) != 0) {
        LOG_ERR("Failed to connect to broker");
        return;
    }

    while (1) {
        int rc;

        rc = mqtt_input(&client);
        if (rc != 0) {
            LOG_ERR("Error in mqtt_input: %d", rc);
            break;
        }

        if (publish(&client) != 0) {
            LOG_ERR("Error publishing");
            break;
        }

        k_sleep(K_MSEC(APP_SLEEP_MSECS));
    }

    mqtt_disconnect(&client);
}