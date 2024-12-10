#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/socket.h>
#include <sys/time.h>
#include <time.h>
#include <zephyr/fs/nvs.h>

int main() {
    // Initialize NVS and other necessary components here

    while (1) {
        // Read data from the MMA8x5x sensor here every 5 seconds

        // Store the read value into NVS for persistence

        // Sleep or set a delay of 5 seconds
    }

    return 0;
}
