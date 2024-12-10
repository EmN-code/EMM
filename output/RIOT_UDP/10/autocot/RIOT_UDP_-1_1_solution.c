#include <thread.h>
#include <udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define PERIOD_MS 5000

static char message_work[] = "work";
static char message_not_work[] = "not work";

void *udp_sender_thread(void *arg) {
    (void)arg;  // Unused parameter
    struct udp_pcb *pcb;
    ip_addr_t server_ip;
    struct pbuf *packet;
    int device_working = 1;  // Example state variable

    // Initialize the UDP communication
    pcb = udp_new();
    if (!pcb) {
        printf("Failed to create PCB\n");
        return NULL;
    }

    if (!ipaddr_aton(SERVER_IP, &server_ip)) {
        printf("Invalid server IP\n");
        udp_remove(pcb);
        return NULL;
    }

    while (1) {
        const char *message = device_working ? message_work : message_not_work;
        packet = pbuf_alloc(PBUF_TRANSPORT, strlen(message), PBUF_RAM);

        if (!packet) {
            printf("Failed to allocate pbuf\n");
            continue;
        }

        memcpy(packet->payload, message, strlen(message));

        if (udp_sendto(pcb, packet, &server_ip, SERVER_PORT) != ERR_OK) {
            printf("Failed to send packet\n");
        }

        pbuf_free(packet);

        device_working = !device_working;  // Toggle state for demonstration

        xtimer_sleep(PERIOD_MS / 1000);
    }
}

int main(void) {
    kernel_pid_t sender_pid;

    sender_pid = thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, udp_sender_thread, NULL, "udp_sender");

    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start sender thread\n");
        return 1;
    }

    return 0;
}