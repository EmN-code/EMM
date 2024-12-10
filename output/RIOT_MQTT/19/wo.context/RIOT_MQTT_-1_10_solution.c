#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>

int main(void) {
    // Initialize network parameters
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Initialize the network
    network.mqttread = NULL;  // Assign appropriate read function
    network.mqttwrite = NULL; // Assign appropriate write function

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set MQTT connection options
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_client";
    connectData.username.cstring = "";
    connectData.password.cstring = "";

    // Connect to the MQTT broker
    int rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    // Publish loop
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = "work";
    message.payloadlen = strlen((char*)message.payload);

    while (1) {
        rc = MQTTPublish(&client, "state", &message);
        if (rc != SUCCESS) {
            printf("Failed to publish, return code %d\n", rc);
        }
        
        // Alternate message
        message.payload = strcmp((char*)message.payload, "work") == 0 ? "not work" : "work";
        message.payloadlen = strlen((char*)message.payload);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}