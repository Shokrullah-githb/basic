
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
    // Iterate through all departments and their units
    for (int i = 0; i < department_count; i++) {
        for (int j = 0; j < departments[i].unit_count; j++) {
            Unit *unit = &departments[i].units[j];

            // Step 1: Move dispatched or returning units
            if (unit->state == UNIT_DISPATCHED || unit->state == UNIT_RETURNING) {
                move_unit_one_step(unit);

                // Step 2: If unit reaches destination
                if (unit->x == unit->target_x && unit->y == unit->target_y) {
                    if (unit->state == UNIT_DISPATCHED) {
                        // Arrived at incident location → Start operating
                        log_unit_arrived(unit, unit->target_x, unit->target_y);
                        unit->state = UNIT_OPERATING;
                        log_unit_state_change(unit, UNIT_OPERATING);
                    } else if (unit->state == UNIT_RETURNING) {
                        // Arrived at base → Start resting instead of waiting
                        log_unit_arrived(unit, unit->target_x, unit->target_y);
                        unit->state = UNIT_RESTING;
                        unit->rest_turns_remaining = 2; // Rest for 2 turns
                        log_unit_state_change(unit, UNIT_RESTING);
                    }
                }
            }

            // Step 3 & 4: Reduce rest turns and switch to waiting
            if (unit->state == UNIT_RESTING) {
                unit->rest_turns_remaining--;
                if (unit->rest_turns_remaining <= 0) {
                    unit->state = UNIT_WAITING;
                    log_unit_state_change(unit, UNIT_WAITING);
                }
            }
        }
    }
}
