#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <stdio.h>

#define STACK_SIZE 1024
#define MQTT_CLIENTID "esp32_client"
#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_client client;
static struct sockaddr_in broker;

static uint8_t rx_buffer[256];
static uint8_t tx_buffer[256];
static uint8_t payload[30];

void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNECTED:
            printf("MQTT client connected!");
            break;

        case MQTT_EVT_DISCONNECTED:
            printf("MQTT client disconnected!");
            break;

        case MQTT_EVT_PUBLISH:
            printf("Message published to topic!");
            break;

        default:
            printf("Unhandled MQTT event: %d", evt->type);
            break;
    }
}

void configure_mqtt(void)
{
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    net_addr_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.broker = (struct sockaddr *)&broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
}

void publish_state(void)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = "esp32/state";
    param.message.topic.topic.size = strlen("esp32/state");
    param.message.payload.data = payload;
    param.message.payload.len = strlen((const char *)payload);
    param.message_id = k_cycle_get_32();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    snprintf(payload, sizeof(payload), "work");

    int rc = mqtt_publish(&client, &param);
    if (rc != 0) {
        printf("Failed to publish message: %d\n", rc);
    }
}

void main(void)
{
    int rc;

    printf("Starting MQTT sample\n");
    configure_mqtt();

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return;
    }
    printf("Connected to MQTT broker\n");

    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}