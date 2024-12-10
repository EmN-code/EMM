#include <zephyr.h>
#include <sys/printk.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <net/net_core.h>
#include <net/net_ip.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define APP_CONNECT_TRIES 10
#define APP_SLEEP_MSECS 5000
#define APP_MQTT_BUFFER_SIZE 128

static uint8_t rx_buffer[APP_MQTT_BUFFER_SIZE];
static uint8_t tx_buffer[APP_MQTT_BUFFER_SIZE];
static struct mqtt_client client;
static struct sockaddr_in broker;
static struct zsock_pollfd fds[1];
static int nfds;

void publish_state(struct mqtt_client *client, enum mqtt_qos qos)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = qos;
    param.message.topic.topic.utf8 = (uint8_t *)"state";
    param.message.topic.topic.size = strlen("state");
    param.message.payload.data = (uint8_t *)"work or not work";
    param.message.payload.len = strlen("work or not work");
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    mqtt_publish(client, &param);
}

void connect_to_broker(void)
{
    struct mqtt_sec_config *tls_cfg = NULL;
    int rc, i = 0;

    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)"zephyr-client";
    client.client_id.size = strlen("zephyr-client");
    client.password = NULL;
    client.user_name = NULL;

    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    while (i++ < APP_CONNECT_TRIES && mqtt_connect(&client) != 0) {
        k_sleep(K_MSEC(APP_SLEEP_MSECS));
    }

    nfds = 1;
    fds[0].fd = client.transport.tcp.sock;
    fds[0].events = ZSOCK_POLLIN;
}

void main(void)
{
    connect_to_broker();

    while (1) {
        publish_state(&client, MQTT_QOS_0_AT_MOST_ONCE);
        k_sleep(K_MSEC(APP_SLEEP_MSECS));
    }
}