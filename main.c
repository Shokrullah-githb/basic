#include "config.h"
#include "log.h"
#include "simulation.h"
#include <stdio.h>

int main() {
    // 1. Initialize the logging system to capture all events
    init_log();
    log_simulation_start();

    // 2. Load the simulation configuration from the file
    // The program expects a file named "config.txt" in the working directory 
    printf("Loading configuration from config.txt...\n");
    load_configuration("sample_config.txt");
    printf("Configuration loaded.\n");
    
    // 3. Start the main simulation loop
    simulation_loop();

    // This part is unreachable because simulation_loop is infinite,
    // but it's good practice. The program exits via the menu.
    log_simulation_end(); 
    return 0;
}