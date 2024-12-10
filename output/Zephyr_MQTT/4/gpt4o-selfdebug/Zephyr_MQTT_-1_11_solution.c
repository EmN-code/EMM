#include <zephyr.h>
#include <zephyr/sys/sys_io.h>
#include <zephyr/net/mqtt.h>
#include <stdio.h>
#include <zephyr/kernel.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)
#define MQTT_CLIENT_ID "esp32_mqtt"

/* Global declaration for MQTT client and buffers */
static struct mqtt_client client;
static uint8_t tx_buffer[128];
static uint8_t rx_buffer[128];
static struct sockaddr_storage broker;

static void prepare_mqtt_client(void)
{
    struct mqtt_client *c = &client;
    struct mqtt_sec_config *cfg = &client.transport.tls.config;

    mqtt_client_init(&client);

    c->broker = &broker;
    mqtt_client_add_subscriber(c, MQTT_CLIENT_ID, sizeof(MQTT_CLIENT_ID));

    /* Initializing buffers. */
    c->rx_buf = rx_buffer;
    c->rx_buf_size = sizeof(rx_buffer);
    c->tx_buf = tx_buffer;
    c->tx_buf_size = sizeof(tx_buffer);

    struct mqtt_utf8 client_id = {
        .utf8 = (uint8_t *)MQTT_CLIENT_ID,
        .size = strlen(MQTT_CLIENT_ID)
    };

    c->client_id = client_id;
}

static int connect_mqtt_broker(void)
{
    int err;
    struct sockaddr_in broker_in = {
        .sin_family = AF_INET,
        .sin_port = htons(BROKER_PORT)
    };

    inet_pton(AF_INET, BROKER_ADDR, &broker_in.sin_addr);

    broker.ss_family = AF_INET;
    memcpy(&broker, &broker_in, sizeof(broker_in));

    mqtt_client_connect(&client);

    /* Wait for the connection to be established */
    do {
        err = mqtt_input(&client);
    } while (err == -EAGAIN);

    return err;
}

static void publish_state(bool state)
{
    struct mqtt_publish_param param;
    uint8_t msg[] = "state: work";

    if (!state) {
        strcpy((char *)msg, "state: not work");
    }

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"esp32/status";
    param.message.topic.topic.size = strlen("esp32/status");
    param.message.payload.data = msg;
    param.message.payload.len = strlen((char *)msg);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void main(void)
{
    int ret;

    printf("Starting MQTT client\n");

    prepare_mqtt_client();
    ret = connect_mqtt_broker();

    if (ret) {
        printf("Connection to broker failed\n");
    } else {
        printf("Successfully connected\n");

        while (1) {
            publish_state(true);
            k_sleep(PUBLISH_INTERVAL);
        }
    }
}