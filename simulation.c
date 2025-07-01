#include "simulation.h"
#include "config.h"
#include "log.h"
#include "map.h"
#include "incident.h"
#include "unit.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep() on Linux/macOS. For Windows, use <windows.h> and Sleep()

void perform_turn_actions() {
    printf("-------------------- Turn Actions --------------------\n");

    // The order of operations in each turn is critical for correct logic.
    // 1. Update unit movements so they reach their destination.
    update_units_movement();

    // 2. Update incident statuses (e.g., start or finish operations) based on unit locations.
    update_incidents();

    // 3. Dispatch available units to new or ongoing incidents.
    dispatch_units();

    // 4. Render the final state of the map for this turn.
    update_and_render();
}

void process_user_input() {
    int choice = 0;
    char input[100];

    while (choice != 2 && choice != 3) {
        printf("\n=== MENU ===\n");
        printf("1. Add New Incident\n");
        printf("2. Continue Simulation\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        fgets(input, sizeof(input), stdin);
        sscanf(input, "%d", &choice);  // ورودی عدد را استخراج می‌کنیم

        switch (choice) {
            case 1: {
                int type_in, priority_in, x, y;
                printf("Enter Incident Type (0:FIRE, 1:MEDICAL, 2:POLICE): ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &type_in);

                printf("Enter Priority (0:LOW, 1:MEDIUM, 2:HIGH): ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &priority_in);

                printf("Enter X and Y coordinates (e.g., 5 8): ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d %d", &x, &y);

                if (type_in >= 0 && type_in <= 2 && priority_in >= 0 && priority_in <= 2) {
                    create_new_incident((IncidentType)type_in, (Priority)priority_in, x, y);
                } else {
                    printf("Invalid input. Please try again.\n");
                }
                break;
            }
            case 2:
                printf("Continuing simulation...\n");
                break;
            case 3:
                printf("Exiting simulation...\n");
                log_simulation_end();
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void simulation_loop() {
    int turn_counter = 0;

    while (1) {
        printf("\n\n===== Turn %d =====\n", turn_counter);

        if (turn_counter == 0) {
            process_user_input(); // منو نشان داده می‌شود

            turn_counter++;       // برو به Turn 1

            continue;             // از این مرحله بپر و برو سر دور بعدی، یعنی Turn 1
        }

        // از Turn 1 به بعد
        perform_turn_actions();

        // منو را هر 10 دور نشان بده (مثلاً Turn 10, 20, ...)
        if (turn_counter % 10 == 0) {
            process_user_input();
        }

        turn_counter++;

        printf("Press Enter to proceed to the next turn...");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}





