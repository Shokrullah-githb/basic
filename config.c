
#include "config.h"
#include "map.h"
#include "unit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_configuration(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open configuration file.\n");
        exit(1);
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MAP_SIZE", 8) == 0) {
            sscanf(line, "MAP_SIZE %d %d", &map_width, &map_height);
        }
        // --------------------------------------------------------------------------IS DONE -------------------------------------------------------------
        // Extract department type, index, coordinates, and unit count.
        // Initialize the department fields accordingly.
        // Then, initialize each unit for the department with appropriate values.
        // Hint: Use sscanf to parse values and a loop to initialize units.

        // Check if the line defines a department
        else if (strncmp(line, "DEPARTMENT", 10) == 0) {
            char type_str[10];
            int index, x, y, unit_count;

            // Parse the department line
            sscanf(line, "DEPARTMENT %s %d %d %d %d", type_str, &index, &x, &y, &unit_count);

            if (department_count < MAX_DEPARTMENTS) {
                // Get a pointer to the next available department slot
                Department *dept = &departments[department_count];

                // Initialize department properties
                dept->type = parse_department_type(type_str); // Convert string to enum
                dept->number = index;
                dept->x = x;
                dept->y = y;
                dept->unit_count = unit_count;

                // Initialize all units within this department
                for (int i = 0; i < dept->unit_count; i++) {
                    if (i < MAX_UNITS_PER_DEPARTMENT) {
                        Unit *unit = &dept->units[i];
                        unit->type = dept->type;
                        unit->departmentNumber = dept->number;
                        unit->unitNumber = i + 1; // Units are 1-indexed
                        unit->x = dept->x;        // Start at department's location
                        unit->y = dept->y;
                        unit->state = UNIT_WAITING; // Initial state
                        unit->target_x = -1;      // No target initially
                        unit->target_y = -1;
                    }
                }
                department_count++; // Increment the global department counter
            }
        }

    }

    fclose(file);
}

IncidentType parse_department_type(const char *str) {
    if (strcmp(str, "FIRE") == 0) return FIRE;
    if (strcmp(str, "MEDICAL") == 0) return MEDICAL;
    return POLICE;
}

Priority parse_priority(const char *str) {
    if (strcmp(str, "LOW") == 0) return LOW;
    if (strcmp(str, "MEDIUM") == 0) return MEDIUM;
    return HIGH;
}