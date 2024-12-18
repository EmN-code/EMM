
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS QOS0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_TIMEOUT 1000
#define MQTT_BUFFER_SIZE 1024
#define MQTT_SEND_BUFFER_SIZE 256
#define MQTT_READ_BUFFER_SIZE 256
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_VERSION 3
#define MQTT_TLS 0
#define MQTT_TLS_CA_CERT NULL
#define MQTT_TLS_CERT NULL
#define MQTT_TLS_KEY NULL
#define MQTT_TLS_PSK_ID NULL
#define MQTT_TLS_PSK NULL
#define MQTT_TLS_PSK_LEN 0
#define MQTT_TLS_PSK_CIPHER NULL
#define MQTT_TLS_PSK_CIPHER_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_LEN 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE 0
#define MQTT_TLS_PSK_CIPHER_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE_MODE