
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define TIMER_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void _mqtt_publish(void)
{
    static int sock = -1;
    static struct mqtt_client client;
    static char rx_buffer[100];
    static char tx_buffer[100];
    static struct mqtt_connection conn;

    if (sock < 0) {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock < 0) {
            puts("Error creating socket");
            return;
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(MQTT_BROKER_PORT);
        inet_pton(AF_INET, MQTT_BROKER_IP, &server_addr.sin_addr);

        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            puts("Error connecting to MQTT broker");
            close(sock);
            sock = -1;
            return;
        }

        mqtt_init(&client, sock, tx_buffer, sizeof(tx_buffer), rx_buffer, sizeof(rx_buffer), NULL);
        mqtt_connect(&client, MQTT_CLIENT_ID, NULL, NULL, 0, NULL, NULL, 0, 400);
    }

    mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), 0, 0);
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        _mqtt_publish();
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    return 0;
}
