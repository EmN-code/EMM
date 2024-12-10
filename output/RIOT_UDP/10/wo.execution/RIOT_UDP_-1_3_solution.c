#include <thread.h>
#include <udp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <MQTTClient.h>
#include <esp_system.h>
#include <event/thread.h>
#include <arpa/inet.h>
#include <dht_params.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 128

static kernel_pid_t udp_thread_pid;
static char udp_thread_stack[THREAD_STACKSIZE_DEFAULT];

void *udp_thread(void *arg) {
    (void)arg;
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = "work";  // This can be changed to "not work" based on your logic

    // Initialize the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        return NULL;  // Handle error appropriately
    }

    while (1) {
        sendto(sock, buffer, sizeof(buffer), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    close(sock);
    return NULL;
}

int main(void) {
    udp_thread_pid =
        thread_create(udp_thread_stack, sizeof(udp_thread_stack),
                      THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                      udp_thread, NULL, "udp_thread");

    thread_join(udp_thread_pid);
    return 0;
}
