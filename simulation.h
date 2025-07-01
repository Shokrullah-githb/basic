
#ifndef SIMULATION_H
#define SIMULATION_H

void simulation_loop();

#endif // SIMULATION_H


// ========================== Simulation Instructions ==========================

// TODO: Implement the main simulation logic in simulation.c based on the following guidelines:

// -------------------- High-Level Simulation Flow --------------------
// 1. The simulation should run in a loop, advancing turns.
// 2. Every 10 turns, show a menu with these options:
//    a. Enter a new incident
//    b. Continue Simulation
//    c. Exit the program
// 3. Process user input to add incidents or control simulation flow.
// 4. On each turn, perform the following actions:
//    - Dispatch units to incidents as needed.
//    - Move each unit one step toward its target if dispatched or returning.
//    - Update incident statuses (WAITING, OPERATION, FINISHED).
//    - Render the updated map after each turn.
// 5. Continue the simulation until the user chooses to exit.

// -------------------- What You Should Do --------------------
// You should implement at least the following functions in simulation.c:
// - void simulation_loop(): Controls the main simulation process.
// - void process_user_input(): Handles menu selections and adding new incidents.
// - void perform_turn_actions(): Dispatches units, moves them, updates incidents, and renders the map.

// Feel free to create additional helper functions to improve code readability and modularity. 
// Better readability and quality of code may result in a higher score

// -------------------- Input/Output Guidelines --------------------
// - Use scanf and printf to handle user input and output.
// - Provide clear prompts and messages for user interactions.
// - Consider adding pauses (e.g., using getchar()) to slow down turn processing for easier visualization.

// NOTE: Be mindful of input validation and handle edge cases gracefully.

// =============================================================================