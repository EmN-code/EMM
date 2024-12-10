#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

void mqtt_event_cb(struct mqtt_client *client,
                   const struct mqtt_evt *evt)
{
    /* Handle different MQTT events here */
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!");
            break;
        case MQTT_EVT_PUBLISH:
            /* Handle message published event */
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!");
            break;
        default:
            break;
    }
}

int mqtt_init(void)
{
    static struct mqtt_client client;
    static uint8_t rx_buffer[128];
    static uint8_t tx_buffer[128];

    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    struct mqtt_sec_config tls_config;
    memset(&tls_config, 0, sizeof(tls_config));

    struct mqtt_client_init_param client_param = {
        .broker = &broker,
        .evt_cb = mqtt_event_cb,
        .rx_buf = rx_buffer,
        .rx_buf_size = sizeof(rx_buffer),
        .tx_buf = tx_buffer,
        .tx_buf_size = sizeof(tx_buffer),
        .clean_session = 1,
    };

    /* Initialize the MQTT client */
    return mqtt_client_init(&client, &client_param);
}

void publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    const char *msg = "work or not work";

    param.message.payload.data = msg;
    param.message.payload.len = strlen(msg);
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

void main(void)
{
    printk("MQTT sample application starting\n");

    int rc = mqtt_init();
    if (rc != 0) {
        printk("Failed to initialize MQTT client, error: %d\n", rc);
        return;
    }

    struct mqtt_client client;
    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to broker, error: %d\n", rc);
        return;
    }

    while (1) {
        publish_state(&client);
        /* Wait for 5 seconds */
        k_sleep(K_SECONDS(5));
    }
}