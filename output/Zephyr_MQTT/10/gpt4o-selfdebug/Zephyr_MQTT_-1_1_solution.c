#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_TOPIC "esp32/state"
#define PUB_INTERVAL 5000  // Interval in milliseconds

/* Buffer sizes for MQTT */
#define MQTT_PAYLOAD_BUFFER_SIZE 128
#define MQTT_BUFFER_SIZE 1024

static struct mqtt_client client;
static uint8_t rx_buffer[MQTT_BUFFER_SIZE];
static uint8_t tx_buffer[MQTT_BUFFER_SIZE];
static uint8_t payload[MQTT_PAYLOAD_BUFFER_SIZE];
static struct sockaddr_storage broker;

static void prepare_mqtt_client(void) {
    mqtt_client_init(&client);

    broker.ss_family = AF_INET;
    net_sin(&broker)->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &net_sin(&broker)->sin_addr);

    struct mqtt_utf8 password = {
        .utf8 = NULL,
        .size = 0
    };

    struct mqtt_utf8 username = {
        .utf8 = NULL,
        .size = 0
    };

    struct mqtt_utf8 client_id = {
        .utf8 = "zephyr_client",
        .size = strlen("zephyr_client")
    };

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id = client_id;
    client.password = password;
    client.user_name = username;
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = MQTT_BUFFER_SIZE;
    client.tx_buf = tx_buffer;
    client.tx_buf_size = MQTT_BUFFER_SIZE;
}

static int publish_state(bool working) {
    struct mqtt_publish_param param;

    snprintf(payload, sizeof(payload), "The device is %s", working ? "working" : "not working");

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)PUB_TOPIC;
    param.message.topic.topic.size = strlen(PUB_TOPIC);
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    return mqtt_publish(&client, &param);
}

void main(void) {
    int rc;
    bool working_state = true;

    prepare_mqtt_client();

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to MQTT broker: %d\n", rc);
        return;
    }

    printk("Successfully connected to MQTT broker\n");

    while (1) {
        rc = publish_state(working_state);
        if (rc != 0) {
            printk("Failed to publish state: %d\n", rc);
        }

        /* Toggle the working state for the next message */
        working_state = !working_state;

        /* Wait for the next cycle */
        k_sleep(K_MSEC(PUB_INTERVAL));
    }

    mqtt_disconnect(&client);
}