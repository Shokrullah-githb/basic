
#include "map.h"
#include "common.h"
#include "incident.h"
#include "unit.h"
#include <stdio.h>
#include <string.h>

MapCell map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];
int map_width = 20, map_height = 10;

void clear_map() {
    // ---------------------------------------------------------DONE: Complete the loop to iterate over the entire map grid.
// For each cell, reset the symbol to " .  " to represent an empty cell.
// Hint: Use two nested loops to cover all rows and columns.


    // Iterate over the entire map grid using two nested loops.
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            // Reset each cell's symbol to represent an empty space.
            sprintf(map[y][x].symbol, " .  ");
        }
    }

}

void render_map() {
    // ----------------------------------------------------------------------------DONE: Implement the logic to print the entire map.
// Hint: Use two nested loops to print each cell's symbol followed by a space.
// Don't forget to print a newline after each row and draw borders if necessary.


    // Print a top border for the map
    printf("\n=== CITY MAP ===\n");

    // Use two nested loops to print each cell's symbol.
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            printf("%s", map[y][x].symbol);
        }
        // Print a newline character after each row to move to the next line.
        printf("\n");
    }

    // Print a bottom border
    printf("================\n");

}

void place_departments() {
    for (int i = 0; i < department_count; i++) {
        char type_char = (departments[i].type == FIRE) ? 'F' :
                         (departments[i].type == MEDICAL) ? 'M' : 'P';
        sprintf(map[departments[i].y][departments[i].x].symbol, "_%c%d_", type_char, departments[i].number);
    }
}

void place_incidents() {
    // -----------------------------------------------------------------DONE: Implement this function to place incidents on the map.
// Only incidents that are not finished should be placed.
// Determine the correct symbol based on incident type and priority.
// Hint: Use sprintf to format the symbol string and assign it to the correct map cell.



    // Iterate through all registered incidents.
    for (int i = 0; i < incident_count; i++) {
        Incident *incident = &incidents[i];

        // Only place incidents that are not finished on the map.
        if (incident->state != INCIDENT_FINISHED) {
            char type_char = (incident->type == FIRE) ? 'F' :
                             (incident->type == MEDICAL) ? 'M' : 'P';
            char priority_char = (incident->priority == LOW) ? 'L' :
                                 (incident->priority == MEDIUM) ? 'M' : 'H';

            // Format the symbol string, e.g., "!FH!" for a high-priority fire.
            sprintf(map[incident->y][incident->x].symbol, "!%c%c! ", type_char, priority_char);
        }
    }

}

void place_units() {
    for (int i = 0; i < department_count; i++) {
        for (int j = 0; j < departments[i].unit_count; j++) {
            Unit *u = &departments[i].units[j];
            if (strcmp(map[u->y][u->x].symbol, " .  ") == 0) {
                char type_char = (u->type == FIRE) ? 'F' :
                                 (u->type == MEDICAL) ? 'M' : 'P';
                sprintf(map[u->y][u->x].symbol, "%c%d-%d", 
                        type_char, u->departmentNumber, u->unitNumber);
            }
        }
    }
}

void update_and_render() {
    // -----------------------------------------------------------------DONE: Determine the correct order of rendering the map.
// Call functions to clear the map, place departments, incidents, and units.
// Finally, call the function that renders the map to the console.



    // The rendering order is crucial for correct visual priority.
    // 1. Clear the map to have a blank slate.
    clear_map();

    // 2. Place units first. They have the lowest priority.
    place_units();

    // 3. Place departments. They will overwrite units if they are at the same location.
    place_departments();

    // 4. Place incidents last. They have the highest visual priority and will overwrite anything else.
    place_incidents();

    // 5. Finally, render the composed map to the console.
    render_map();

}