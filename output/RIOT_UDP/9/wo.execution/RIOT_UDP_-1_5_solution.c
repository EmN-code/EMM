#include <mutex.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <esp_wifi.h>
#include <dht_params.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <esp_system.h>
#include <thread.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

void *udp_sender_thread(void *arg) {
    (void)arg;
    char *message_state[] = {"work", "not work"};
    int toggle = 0;

    while (1) {
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            printf("Error creating socket\n");
            continue;
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);
        inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

        if (sendto(sock, message_state[toggle], strlen(message_state[toggle]), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printf("Error sending message\n");
        }

        toggle = !toggle; // Toggle between "work" and "not work"

        close(sock);

        vTaskDelay(5000 / portTICK_PERIOD_MS); // Delay for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Initialize WiFi
    esp_err_t ret = esp_wifi_init(NULL);
    if (ret != ESP_OK) {
        printf("Error initializing WiFi\n");
        return -1;
    }

    // Create and start the UDP sender thread
    char udp_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(udp_stack, sizeof(udp_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  udp_sender_thread, NULL, "udp_sender");

    return 0;
}
