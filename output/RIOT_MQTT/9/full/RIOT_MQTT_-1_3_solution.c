#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <event/thread.h>
#include <thread.h>
#include <net/emcute.h>
#include <MQTTClient.h> // Including MQTTClient.h as the last

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STACK_SIZE 4096
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];

void *publisher_thread(void *arg) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    /* Initialize connection parameters */
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying in 1 second...\n");
            continue;
        } else {
            printf("Network connection succeeded.\n");
            break;
        }
    }

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying in 1 second...\n");
            continue;
        } else {
            printf("MQTT connection succeeded.\n");
            break;
        }
    }

    while (1) {
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen("work");

        rc = MQTTPublish(&client, "state", &message);
        if (rc != 0) {
            printf("MQTT publish failed. Retrying in 1 second...\n");
        } else {
            printf("Message published: work\n");
        }

        // Delay 5 seconds (using a hypothetical delay function)
        my_delay_function(5000);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0, publisher_thread, NULL, "publisher");
    while (1) {
        // Main thread can perform other tasks or sleep
    }
    return 0;
}