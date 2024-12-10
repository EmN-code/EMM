
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/printk.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static int send_coap_request(int sock, const struct sockaddr *addr)
{
    struct coap_packet request;
    uint8_t buffer[128];
    int ret;

    ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 8,
                           coap_next_token(), COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to initialize CoAP packet\n");
        return ret;
    }

    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    if (ret < 0) {
        printk("Failed to append URI path option\n");
        return ret;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker\n");
        return ret;
    }

    ret = coap_packet_append_payload(&request, MESSAGE, strlen(MESSAGE));
    if (ret < 0) {
        printk("Failed to append payload\n");
        return ret;
    }

    ret = sendto(sock, request.data, request.offset, 0, addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        return ret;
    }

    printk("CoAP request sent successfully\n");
    return 0;
}

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    int ret;

    // Initialize WiFi
    struct net_if *iface = net_if_get_default();
    if (!iface) {
        printk("Failed to get default network interface\n");
        return -1;
    }

    // Connect to WiFi (assuming WiFi is already configured)
    wifi_connect(iface, "your_ssid", "your_password", NULL);

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Send the CoAP request
    ret = send_coap_request(sock, (struct sockaddr *)&server_addr);
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    // Close the socket
    close(sock);

    printk("CoAP request sent successfully\n");
    return 0;
}
