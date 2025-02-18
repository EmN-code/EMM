#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "timex.h"
#include "ztimer.h"
#include "shell.h"
#include "thread.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "periph/gpio.h" 

#define THREAD_STACKSIZE        (THREAD_STACKSIZE_IDLE)

#define LED_GPIO GPIO12
#define BUF_SIZE                        1024
#define MQTT_VERSION_v311               4       /* MQTT v3.1.1 version is 4 */
#define COMMAND_TIMEOUT_MS              4000


/**
 * @brief Default MQTT port
 */
#define DEFAULT_MQTT_PORT               1883

/**
 * @brief Keepalive timeout in seconds
 */
#define DEFAULT_KEEPALIVE_SEC           10

#ifndef MAX_LEN_TOPIC
#define MAX_LEN_TOPIC                   100
#endif

#ifndef MAX_TOPICS
#define MAX_TOPICS                      4
#endif

#define IS_CLEAN_SESSION                1
#define IS_RETAINED_MSG                 0

static MQTTClient client;
static Network network;

char* DEFAULT_MQTT_CLIENT_ID = "esp32_test";
char* DEFAULT_MQTT_USER = "esp32";
char* DEFAULT_MQTT_PWD = "esp32";
char* DEFAULT_MQTT_IP = "47.102.103.1";
char* DEFAULT_TOPIC = "v1/devices/me/telemetry";

int mqtt_discon(void)
{

    int res = MQTTDisconnect(&client);
    if (res < 0) {
        printf("mqtt_example: Unable to disconnect\n");
    }
    else {
        printf("mqtt_example: Disconnect successful\n");
    }

    NetworkDisconnect(&network);
    return res;
}

int mqtt_connect(void)
{

    char *remote_ip = (char*)DEFAULT_MQTT_IP;

    int ret = -1;

    /* ensure client isn't connected in case of a new connection */
    if (client.isconnected) {
        printf("mqtt_example: client already connected, disconnecting it\n");
        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }

    int port = DEFAULT_MQTT_PORT;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = MQTT_VERSION_v311;

    data.clientID.cstring = (char *)DEFAULT_MQTT_CLIENT_ID;
    data.username.cstring = (char *)DEFAULT_MQTT_USER;
    data.password.cstring = (char *)DEFAULT_MQTT_PWD;
    data.keepAliveInterval = DEFAULT_KEEPALIVE_SEC;
    data.cleansession = IS_CLEAN_SESSION;
    data.willFlag = 0;

    printf("mqtt_example: Connecting to MQTT Broker from %s %d\n",
            remote_ip, port);
    printf("mqtt_example: Trying to connect to %s, port: %d\n",
            remote_ip, port);

    ret = NetworkConnect(&network, remote_ip, port);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect\n");
        return ret;
    }

    printf("user:%s clientId:%s password:%s\n", data.username.cstring,
             data.clientID.cstring, data.password.cstring);
    ret = MQTTConnect(&client, &data);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect client %d\n", ret);
        return ret;
    }
    else {
        printf("mqtt_example: Connection successfully\n");
    }

    return (ret > 0) ? 0 : 1;
}

int mqtt_pub(int temp)
{
    enum QoS qos = QOS0;

    MQTTMessage message;
    message.qos = qos;
    message.retained = IS_RETAINED_MSG;
    char json[200];  
    sprintf(json, "{temperature:%d}", temp);
    printf("[Send] Message:%s\n", json);

    message.payload = json;
    message.payloadlen = strlen((char *)message.payload);

    int rc;
    if ((rc = MQTTPublish(&client, DEFAULT_TOPIC, &message)) < 0) {
        printf("mqtt_example: Unable to publish (%d)\n", rc);
    }
    else {
        printf("mqtt_example: Message (%s) has been published to topic %s"
               "with QOS %d\n",
               (char *)message.payload, DEFAULT_TOPIC, (int)message.qos);
    }

    return rc;
}




void *_alarm(void *arg){
    (void) arg;
    int led_state = 1;
    for (int i = 0; i < 10 * 2; ++i)
    {
        led_state = !led_state;
        gpio_write(LED_GPIO, led_state);
        ztimer_sleep(ZTIMER_MSEC, 1 * MS_PER_SEC);
    }
    return NULL;
}

static unsigned char buf[BUF_SIZE];
static unsigned char readbuf[BUF_SIZE];

int main(void)
{

    gpio_init(LED_GPIO, GPIO_OUT);
    NetworkInit(&network);
    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE,
                   readbuf, BUF_SIZE);
    MQTTStartTask(&client);
    ztimer_sleep(ZTIMER_MSEC, 6 * MS_PER_SEC);
    char stack[THREAD_STACKSIZE];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, _alarm, NULL, "alarm");

    while(1){

        if (mqtt_connect()) {
            int temp = 22.7;
            mqtt_pub(temp);
            ztimer_sleep(ZTIMER_MSEC, 5 * MS_PER_SEC);
            mqtt_discon();
        }
    }


    return 0;
}
