#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <stdio.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "zephyr_client"
#define MQTT_TOPIC "state/topic"
#define MQTT_MSG_WORK "work"
#define MQTT_MSG_NOT_WORK "not work"
#define STACK_SIZE 1024

K_THREAD_STACK_DEFINE(stack_area, STACK_SIZE);

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct pollfd fds;

static void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected\n");
        break;
    default:
        printk("MQTT event received: %d\n", evt->type);
        break;
    }
}

static void prepare_fds(void) {
    fds.fd = client.transport.tcp.sock;
    fds.events = POLLIN;
}

static int broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);

    return zsock_inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);
}

static void mqtt_publish_message(struct mqtt_client *client, const char *msg) {
    struct mqtt_publish_param param;

    param.message.payload.data = msg;
    param.message.payload.len = strlen(msg);
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.dup_flag = false;
    param.retain_flag = false;
    param.message_id = 1;

    mqtt_publish(client, &param);
}

static void mqtt_thread(void) {
    int rc;

    mqtt_client_init(&client);

    rc = broker_init();
    if (rc != 0) {
        printk("Failed to initialize broker address\n");
        return;
    }

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;

    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to MQTT broker\n");
        return;
    }

    prepare_fds();

    while (1) {
        rc = poll(&fds, 1, K_SECONDS(5));  // Wait for 5 seconds
        if (rc > 0 && (fds.revents & POLLIN) != 0) {
            mqtt_input(&client);
        }

        mqtt_live(&client);

        // Publish state to the topic
        mqtt_publish_message(&client, MQTT_MSG_WORK);

        k_sleep(K_SECONDS(5));
    }
}

void main(void) {
    k_thread_create(stack_area, STACK_SIZE, (k_thread_entry_t)mqtt_thread,
                    NULL, NULL, NULL, K_PRIO_PREEMPT(7), 0, K_NO_WAIT);
}