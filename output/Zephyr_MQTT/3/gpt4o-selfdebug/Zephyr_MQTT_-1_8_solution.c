#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <net/net_config.h>
#include <stdio.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENTID "zephyr_client"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct sockaddr_storage broker;

static void prepare_mqtt_client(void) {
    struct mqtt_utf8 username = {
        .utf8 = NULL,
        .size = 0
    };
    struct mqtt_utf8 password = {
        .utf8 = NULL,
        .size = 0
    };
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;

    /* Broker Configuration */
    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr->sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.password = &password;
    client.user_name = &username;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

static void connect_to_broker(void) {
    /* Connect to the MQTT broker */
    int rc;
    rc = mqtt_connect(&client);
    if (rc != 0) {
        printf("Failed to connect to broker: %d\n", rc);
    }
}

static void publish_state(const char *state) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"state";
    param.message.topic.topic.size = strlen("state");
    param.message.payload.data = (uint8_t *)state;
    param.message.payload.len = strlen(state);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    mqtt_publish(&client, &param);
}

void main(void) {
    prepare_mqtt_client();
    connect_to_broker();

    while (1) {
        publish_state("work");
        k_sleep(PUBLISH_INTERVAL);
    }
}