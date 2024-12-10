#include <thread.h>
#include <timex.h>
#include <paho_mqtt.h>
#include <esp_event.h>
#include <MQTTClient.h>

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * US_PER_SEC)

static void* mqtt_thread(void* arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        return NULL;
    }

    message.qos = QOS0;
    message.retained = false;
    message.payload = "work";
    message.payloadlen = strlen(message.payload);

    while (1) {
        MQTTMessage publishMessage = message;
        MQTTPublish(&client, "state", &publishMessage);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(NULL, 8);
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}