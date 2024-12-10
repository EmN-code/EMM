#include <esp_system.h>

// Function declarations and definitions
void initialize_system();
void update_nvs_simulation(int new_state);
void simulate_periodic_task();

// Simulated NVS state store
int simulated_nvs_state = 0;

void initialize_system() {
    // Simulate system initialization
    printf("System initialized.\n");
}

void update_nvs_simulation(int new_state) {
    // Simulate updating NVS storage with the new state
    simulated_nvs_state = new_state;
    printf("Updated simulated NVS state to: %d\n", simulated_nvs_state);
}

void simulate_periodic_task() {
    // Simulate updating the system every second
    for (int i = 0; i < 10; ++i) { // Run for 10 seconds simulation
        int new_state = (simulated_nvs_state == 0) ? 1 : 0;
        update_nvs_simulation(new_state);
        sleep(1); // Sleep for 1 second to simulate delay
    }
}

int main() {
    initialize_system();
    simulate_periodic_task();
    return 0;
}