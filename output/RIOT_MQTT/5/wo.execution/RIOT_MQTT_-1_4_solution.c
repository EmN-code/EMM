#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <paho_mqtt.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <dht_params.h>
#include <dht.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define TOPIC "device/state"
#define INTERVAL (5U * US_PER_SEC) // Publish interval in microseconds

static ztimer_t timer;
static MQTTClient client;
static Network network;
unsigned char sendbuf[100], readbuf[100];

static void publish_state(void) {
    const char *state = "work";
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    MQTTPublish(&client, TOPIC, &message);
}

void init_mqtt(void) {
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_PORT);

    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32-client";

    if (MQTTConnect(&client, &data) != 0) {
        // Handle failed connection
    }
}

int main(void) {
    esp_err_t ret = esp_wifi_init();
    if (ret != ESP_OK) {
        printf("WiFi init failed\n");
        return -1;
    }

    esp_event_loop_create_default();  // Updated function call

    init_mqtt();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }
    return 0;
}