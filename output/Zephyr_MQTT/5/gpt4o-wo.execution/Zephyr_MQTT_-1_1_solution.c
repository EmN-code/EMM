#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <stdio.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param pub_param;

static void prepare_fds(struct mqtt_client *client, struct zsock_pollfd *fds)
{
    fds->fd = client->transport.tcp.sock;
    fds->events = ZSOCK_POLLIN;
}

static void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printf("MQTT connected!");
        break;
    default:
        break;
    }
}

static int publish(struct mqtt_client *client)
{
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = "state";
    pub_param.message.topic.topic.size = strlen("state");
    pub_param.message.payload.data = "work";  // or "not work" based on some logic
    pub_param.message.payload.len = strlen(pub_param.message.payload.data);
    pub_param.message_id = sys_rand32_get();
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;

    return mqtt_publish(client, &pub_param);
}

int main(void)
{
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;
    broker_addr->sin_family = AF_INET;
    broker_addr->sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker_addr->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_pub_client";
    client.client_id.size = strlen(client.client_id.utf8);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printf("MQTT connect error: %d", rc);
        return 0;
    }

    struct zsock_pollfd fds;
    prepare_fds(&client, &fds);

    while (1) {
        if (zsock_poll(&fds, 1, K_MSEC(100)) >= 0 && (fds.revents & ZSOCK_POLLIN)) {
            mqtt_input(&client);
            mqtt_live(&client);
        }

        publish(&client);

        k_sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&client);

    return 0;
}