#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>

LOG_MODULE_REGISTER(mqtt_pub_example, LOG_LEVEL_DBG);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define STACK_SIZE 4096
#define THREAD_PRIORITY 7

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param pub_param;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct zsock_pollfd fds[1];
static int nfds;

void prepare_fds(void)
{
    fds[0].fd = client.transport.tcp.sock;
    fds[0].events = ZSOCK_POLLIN;
    nfds = 1;
}

void mqtt_evt_handler(struct mqtt_client *c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        LOG_INF("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT client disconnected!");
        break;
    default:
        break;
    }
}

void publish_state(void)
{
    static char msg[] = "work";
    pub_param.message.payload.data = msg;
    pub_param.message.payload.len = strlen(msg);
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = "state";
    pub_param.message.topic.topic.size = strlen("state");
    pub_param.retain_flag = 0;
    pub_param.dup_flag = 0;

    mqtt_publish(&client, &pub_param);
    LOG_INF("Published: %s", msg);
}

void mqtt_thread(void)
{
    mqtt_input(&client);
    if (zsock_poll(fds, nfds, K_MSEC(5000)) < 0) {
        LOG_ERR("poll error");
    } else if (fds[0].revents & ZSOCK_POLLIN) {
        mqtt_input(&client);
    }
    publish_state();
}

void main(void)
{
    int rc;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *) "zephyr_publisher";
    client.client_id.size = strlen("zephyr_publisher");
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    rc = mqtt_connect(&client);
    if (rc != 0) {
        LOG_ERR("mqtt_connect failed: %d", rc);
        return;
    }

    prepare_fds();

    k_thread_create(&mqtt_thread, STACK_SIZE, THREAD_PRIORITY, 0, NULL);
}