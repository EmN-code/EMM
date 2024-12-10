#include "mqtt_client.h"
#include "net.h"
#include "msg.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "device/state"
#define PUBLISH_INTERVAL_S (5U)

static mqtt_client_t client;
static const char *mqtt_broker_ip = MQTT_BROKER_IP;
static int mqtt_broker_port = MQTT_BROKER_PORT;

static void on_mqtt_event_default(mqtt_client_t *client, void *arg, mqtt_event_t event)
{
    /* Handle MQTT events */
    (void)client;
    (void)arg;
    (void)event;
}

static void *mqtt_publisher_thread(void *arg)
{
    msg_t msg;
    (void)arg;

    while (1) {
        char payload[32];
        snprintf(payload, sizeof(payload), "state: work\n");
        mqtt_publish(&client, 
                     MQTT_PUBLISH_TOPIC, 
                     payload, strlen(payload), 
                     0);

        msg_receive(&msg);
        xtimer_sleep(PUBLISH_INTERVAL_S);
    }

    return NULL;
}

static int mqtt_connect(void)
{
    mqtt_client_params_t params;

    params.client_id = "esp32-device";
    params.user = NULL;  // No authentication
    params.pass = NULL;  // No password
    params.clean_session = true;
    params.keep_alive = 10;
    params.timeout = 1000;
    params.transport = MQTT_TRANSPORT_OVER_TCP;
    params.event_cb = on_mqtt_event_default;
    
    mqtt_connect(&client, mqtt_broker_ip, mqtt_broker_port, &params);

    return 0;
}

int main(void)
{
    printf("MQTT publish example started\n");

    msg_init_queue(mqtt_publisher_thread, sizeof(mqtt_publisher_thread)/sizeof(msg_t));

    mqtt_connect();

    thread_create(NULL, 0, THREAD_FLAG_STACKMARKER, THREAD_PRIORITY_MAIN + 1,
                  mqtt_publisher_thread, NULL, "mqtt_publisher");
    return 0;
}