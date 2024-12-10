#include "thread.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL_MS (5000U)

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *mqtt_thread(void *arg) {
    (void)arg;

    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    unsigned char sendbuf[128], readbuf[128];

    NetworkInit(&network);
    NetworkConnect(&network, SERVER_IP, SERVER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != 0) {
        return NULL;
    }

    MQTTMessage message;
    char *topic = "esp32/status";

    while (1) {
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (unsigned char *)"work";
        message.payloadlen = strlen((char *)message.payload);

        MQTTPublish(&client, topic, &message);

        xtimer_sleep(PUB_INTERVAL_MS / 1000);
    }

    NetworkDisconnect(&network);
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}