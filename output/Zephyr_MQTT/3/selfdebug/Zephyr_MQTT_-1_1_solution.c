#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct zsock_pollfd fds[1];

static void prepare_fds(struct mqtt_client *client) {
    fds[0].fd = client->transport.tcp.sock;
    fds[0].events = ZSOCK_POLLIN;
}

static void mqtt_event_handler(struct mqtt_client *const client,
                               const struct mqtt_evt *event) {
    switch (event->type) {
    case MQTT_EVT_CONNACK:
        printk("Connected to MQTT broker\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("Disconnected from MQTT broker\n");
        break;
    default:
        break;
    }
}

static int connect_to_broker(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = "zephyr_mqtt_client";
    client.client_id.size = strlen(client.client_id.utf8);
    client.protocol_version = MQTT_VERSION_3_1_1;

    mqtt_client_init(&client);

    return mqtt_connect(&client);
}

static int publish_state(const char *state) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = "state/topic";
    param.message.topic.topic.size = strlen(param.message.topic.topic.utf8);
    param.message.payload.data = state;
    param.message.payload.len = strlen(state);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    return mqtt_publish(&client, &param);
}

int main(void) {
    printk("Connecting to MQTT broker\n");
    if (connect_to_broker() != 0) {
        printk("Failed to connect to MQTT broker\n");
        return -1;
    }

    prepare_fds(&client);

    while (1) {
        int rc = zsock_poll(fds, 1, PUBLISH_INTERVAL);
        if (rc < 0) {
            printk("poll error: %d\n", errno);
            break;
        }
        if (fds[0].revents & ZSOCK_POLLIN) {
            mqtt_input(&client);
        }
        publish_state("work");
        k_sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&client);
    return 0;
}