#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/socket.h>
#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "zephyr_esp32"

static struct mqtt_client client;
static struct sockaddr_storage broker_address;

void init_mqtt_client(void) {
    mqtt_client_init(&client);

    /* Socket transport configuration */
    struct mqtt_transport transport = {
        .type = MQTT_TRANSPORT_NON_SECURE,
        .tcp.sock = -1
    };

    client.transport = transport;
    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.broker = &broker_address;

    client.evt_cb = NULL;
    client.clean_session = true;
}

void connect_to_broker(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker_address;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printf("MQTT connect failed: %d\n", rc);
    }
}

void publish_state(void) {
    static const struct mqtt_publish_param param = {
        .message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE,
        .message.topic.topic.utf8 = "state",
        .message.topic.topic.size = strlen("state"),
        .message.payload.data = "work",
        .message.payload.len = strlen("work"),
        .message_id = 0,
        .dup_flag = 0,
        .retain_flag = 0
    };

    mqtt_publish(&client, &param);
}

void main(void) {
    printf("Starting MQTT sample\n");

    init_mqtt_client();
    connect_to_broker();

    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}