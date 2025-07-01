
#include "unit.h"
#include "map.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>

Department departments[MAX_DEPARTMENTS];
int department_count = 0;

int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void move_unit_one_step(Unit *u) {
    // Just for debugging purposes. Remove after complete implementation.
    printf("Moving Unit %d-%d\n", u->departmentNumber, u->unitNumber);

    // ----------------------------------------------------------DONE : Implement logic to move the unit one step toward its target.
    // Only move along one axis per function call. Prioritize X direction.
    // Hint: Use simple conditionals to decide whether to increment or decrement X or Y.

    // Prioritize moving in the X direction first
    if (u->x < u->target_x) {
        u->x++;
    } else if (u->x > u->target_x) {
        u->x--;
    }
    // If already aligned on the X axis, move in the Y direction
    else if (u->y < u->target_y) {
        u->y++;
    } else if (u->y > u->target_y) {
        u->y--;
    }

}

void update_units_movement() {
    // ------------------------------------------------------------DONE: Iterate through all departments and units.
    // For each unit in DISPATCHED or RETURNING state, move it one step toward its target.
    // If the unit reaches its target, update its state accordingly.
    // Don't forget to log the state changes using the appropriate logging function.

    // Iterate through all departments and their units
    for (int i = 0; i < department_count; i++) {
        for (int j = 0; j < departments[i].unit_count; j++) {
            Unit *unit = &departments[i].units[j];

            // Only move units that are actively dispatched or returning
            if (unit->state == UNIT_DISPATCHED || unit->state == UNIT_RETURNING) {
                // Move the unit one step closer to its target
                move_unit_one_step(unit);

                // Check if the unit has arrived at its destination
                if (unit->x == unit->target_x && unit->y == unit->target_y) {
                    if (unit->state == UNIT_DISPATCHED) {
                        // Unit has arrived at the incident
                        log_unit_arrived(unit, unit->target_x, unit->target_y);
                        unit->state = UNIT_OPERATING;
                        log_unit_state_change(unit, UNIT_OPERATING);
                    } else if (unit->state == UNIT_RETURNING) {
                        // Unit has returned to its department base
                        log_unit_arrived(unit, unit->target_x, unit->target_y); // Log arrival at base
                        unit->state = UNIT_WAITING;
                        log_unit_state_change(unit, UNIT_WAITING);
                    }
                }
            }
        }
    }

}