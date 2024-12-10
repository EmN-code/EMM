#include <architecture.h> 
#include <arpa/inet.h> 
#include <dht.h>
#include <dht_params.h> 
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <esp_system.h> 
#include <esp_wifi.h> 
#include <kernel_defines.h>
#include <mbox.h>
#include <mma8x5x.h> 
#include <mma8x5x_params.h> 
#include <mma8x5x_regs.h> 
#include <msg.h>
#include <mutex.h> 
#include <net/emcute.h>
#include <net/gcoap.h> 
#include <net/sock/udp.h> 
#include <net/sock/util.h> 
#include <netinet/in.h>
#include <paho_mqtt.h> 
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h> 
#include <sys/socket.h> 
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <ztimer.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static msg_t msg_queue[8];

int main(void) {
    msg_init_queue(msg_queue, 8);

    Network network;
    MQTTClient client;

    unsigned char sendbuf[100], readbuf[100];

    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        MQTTMessage message;
        message.qos = 1;
        message.retained = 0;
        message.payload = (void*)"work or not work";
        message.payloadlen = strlen((char*)message.payload);

        MQTTPublish(&client, "state/topic", &message);
        printf("Message published: work or not work\n");

        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
