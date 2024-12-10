#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/init.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE 	"Hello, COAP Cloud"
#define STACKSIZE 1024

static struct k_work_delayable coap_post_work;
static const struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(SERVER_PORT),
    .sin_addr = {
        .s_addr = inet_addr(SERVER_ADDR)
    }
};

static int client_fd;

void send_coap_post(struct k_work *work)
{
    struct coap_packet request;
    uint8_t request_data[128];

    int r = coap_packet_init(&request, request_data, sizeof(request_data),
                             COAP_VERSION_1,
                             COAP_TYPE_CON,
                             COAP_TOKEN_LEN);
    if (r < 0) {
        printk("Failed to initialize CoAP packet\n");
        return;
    }

    coap_packet_append_token(&request, (uint8_t *)&r, sizeof(r));

    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, strlen(COAP_URI_PATH));

    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("Failed to append payload marker\n");
        return;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)COAP_MESSAGE, strlen(COAP_MESSAGE));
    if (r < 0) {
        printk("Failed to append payload\n");
        return;
    }

    r = sendto(client_fd, request_data, request.offset, 0,
               (struct sockaddr *)&server_addr,
               sizeof(server_addr));

    if (r < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent\n");
    }

    k_work_reschedule(&coap_post_work, K_SECONDS(10));
}

static void init_coap_client(void)
{
    client_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_fd < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    k_work_init_delayable(&coap_post_work, send_coap_post);
    
    send_coap_post(NULL);
}

int main(void)
{
    printk("Starting CoAP client example\n");
    init_coap_client();
    return 0;
}
