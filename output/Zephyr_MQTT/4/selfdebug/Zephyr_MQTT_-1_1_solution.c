#include <zephyr.h>
#include <string.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <errno.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;

#define MQTT_CLIENTID CONFIG_MQTT_CLIENT_ID

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static struct mqtt_publish_param pub_param;

static void prepare_fds(struct mqtt_client *client, struct pollfd *fds)
{
    fds->fd = client->transport.tcp.sock;
    fds->events = POLLIN;

    if (client->internal.state == MQTT_STATE_CONNECTED) {
        fds->events |= POLLOUT;
    }
}

static int try_to_connect(struct mqtt_client *client)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);

    inet_pton(AF_INET, BROKER_ADDRESS, &broker4->sin_addr);

    client->broker = &broker;

    return mqtt_connect(client);
}

static void mqtt_evt_handler(struct mqtt_client *const client,
			    const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!");
        break;

    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!");
        break;

    default:
        break;
    }
}

static void configure_mqtt(void)
{
    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (const uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

static void publish_state(bool work)
{
    const char *state = work ? "work" : "not work";
    pub_param.message.payload.data = state;
    pub_param.message.payload.len = strlen(state);
    pub_param.message.topic.topic.utf8 = (uint8_t *)"state";
    pub_param.message.topic.topic.size = strlen("state");
    pub_param.message_id++; // Increment the message ID

    mqtt_publish(&client, &pub_param);
}

void main(void)
{
    int rc;
    struct pollfd fds;
    bool work = true;

    configure_mqtt();

    rc = try_to_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to broker (err %d)", rc);
        return;
    }

    prepare_fds(&client, &fds);

    while (1) {
        if (poll(&fds, 1, mqtt_keepalive_time_left(&client))) {
            mqtt_input(&client);
            mqtt_live(&client);

            if ((fds.revents & POLLOUT) == POLLOUT) {
                publish_state(work);

                /* Toggle state for next publish */
                work = !work;
            }
        }

        /* Sleep until next publish cycle */
        k_sleep(PUBLISH_INTERVAL);
    }
}