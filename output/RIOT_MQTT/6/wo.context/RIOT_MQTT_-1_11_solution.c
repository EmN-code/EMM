#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <net/sock.h>
#include <paho_mqtt.h> // Assuming paho_mqtt.h provides the required MQTT functions

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_PUB_TOPIC "device/state"
#define MQTT_CLIENT_ID "RIOT_ESP32"
#define PUBLISH_INTERVAL (5U * 1000U) // 5 seconds in milliseconds

static ztimer_t timer;
static bool state = false;  // Work state
static mqtt_client_t mqtt_client;
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

static void on_pub_request_complete(void *arg, mqtt_client_t *client, mqtt_pub_t *pub_resp) {
    puts("Publish request completed");
}

static void publish_state(void) {
    char message[20];
    state = !state;
    sprintf(message, "State: %s", state ? "WORK" : "NOT WORK");
    mqtt_pub_t pub_packet;
    pub_packet.qos = MQTT_QOS_0;
    pub_packet.retain = 0;
    pub_packet.topic = MQTT_PUB_TOPIC;
    pub_packet.payload = message;
    pub_packet.payload_len = strlen(message);
    int res = mqtt_pub(&mqtt_client, &pub_packet);
    if (res != 0) {
        printf("Error publishing message: %d\n", res);
    }
}

static void timer_callback(void *arg) {
    (void)arg;
    publish_state();
}

int main(void) {
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    mqtt_opt_t mqtt_opts;
    mqtt_opts.clean_session = 1;
    mqtt_opts.keep_alive = 5U;

    if (mqtt_connect(&mqtt_client, &gw, MQTT_CLIENT_ID, &mqtt_opts) != 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    ztimer_set(ZTIMER_MSEC, &timer, PUBLISH_INTERVAL);
    publish_state();

    while (1) {
        ztimer_time_t now = ztimer_now(ZTIMER_MSEC);
        if (ztimer_remaining(ZTIMER_MSEC, &timer) == 0) {
            publish_state();
            ztimer_set(ZTIMER_MSEC, &timer, PUBLISH_INTERVAL);
        }
        ztimer_sleep_until(ZTIMER_MSEC, now + 1);
    }

    return 0;
}