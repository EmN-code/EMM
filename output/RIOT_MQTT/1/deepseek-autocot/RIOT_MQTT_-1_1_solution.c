
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void publish_state(void)
{
    static int state = 0;
    const char *payload = (state++ % 2 == 0) ? "work" : "not work";

    mqtt_client_t client;
    mqtt_client_config_t config = {
        .host = BROKER_IP,
        .port = BROKER_PORT,
        .client_id = "riot_esp32",
        .clean_session = 1,
    };

    mqtt_client_init(&client, &config);

    mqtt_publish_config_t pub_config = {
        .topic = TOPIC,
        .payload = payload,
        .payload_len = strlen(payload),
        .qos = MQTT_QOS_0,
        .retain = 0,
    };

    mqtt_client_publish(&client, &pub_config);
    mqtt_client_disconnect(&client);
}

int main(void)
{
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
