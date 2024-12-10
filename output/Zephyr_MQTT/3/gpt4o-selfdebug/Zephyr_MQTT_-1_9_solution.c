#include <zephyr/types.h>
#include <zephyr.h>
#include <sys/printk.h>
#include <net/socket.h>
#include <random/rand32.h>
#include <net/mqtt.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "zephyr_client"

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_client client;
static struct sockaddr_in broker;

static void prepare_mqtt_client(void) {
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.client_id.utf8 = (uint8_t *)CLIENT_ID;
    client.client_id.size = strlen(CLIENT_ID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

static void publish_state(const char *state) {
    struct mqtt_publish_param param;

    memset(&param, 0, sizeof(param));
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"state";
    param.message.topic.topic.size = strlen("state");
    param.message.payload.data = (uint8_t *)state;
    param.message.payload.len = strlen(state);
    param.message_id = sys_rand32_get();

    mqtt_publish(&client, &param);
}

void main(void) {
    int rc;

    prepare_mqtt_client();

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("mqtt_connect failed: %d\n", rc);
        return;
    }

    while (1) {
        int64_t uptime = k_uptime_get();
        const char *state = (uptime % 2 == 0) ? "work" : "not work";
        publish_state(state);
        k_sleep(K_SECONDS(5));
    }
}