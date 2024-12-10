#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <paho_mqtt.h>
#include <mutex.h>
#include <MQTTClient.h>
#include <esp_wifi.h>
#include <esp_event.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void publish_state(void) {
    MQTTMessage message;
    char payload[] = "work";

    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(&client, "state", &message);
}

int main(void) {
    esp_wifi_init();
    esp_event_loop_init(NULL); // Initialize event loop

    ztimer_ticks_t last_wakeup = ztimer_now(ZTIMER_USEC);
    
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_client";

    MQTTConnect(&client, &data);

    while (1) {
        publish_state();
        ztimer_sleep_until(ZTIMER_USEC, &last_wakeup, MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}