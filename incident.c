
#include "incident.h"

#include <stdbool.h>

#include "log.h"
#include "unit.h"
#include "map.h"
#include <stdio.h>
#include <string.h>

Incident *incidents = NULL;
int incident_count = 0;
int incident_capacity = 0;

// ---------------------------------------------------------------DONE!: Initiate an array for storing incident requirements for different incident types and different priorities
// HINT: Use a 2d array of incident requirements like this:
// HINT:               Incident Requirement : {fire_units, medical_units, police_units, time_required}
//                         LOW      MEDIUM    HIGH
//            FIRE    [ {1,0,0,3} {1,1,0,6} {1,1,1,8}
//            MEDICAL   {0,1,0,1} {0,1,0,3} {0,1,0,5}
//            POLICE    {0,0,1,2} {0,1,1,4} {0,1,1,8} ]
//


// This 2D array stores the requirements for each incident type and priority.
// The structure is {fire_units, medical_units, police_units, time_required}.
// Dimensions are [IncidentType][Priority].
const IncidentRequirement incident_requirements[3][3] = {
    // Index 0: FIRE incidents
    {
        {1, 0, 0, 3}, // LOW priority
        {1, 1, 0, 6}, // MEDIUM priority
        {1, 1, 1, 8}  // HIGH priority
    },
    // Index 1: MEDICAL incidents
    {
            {0, 1, 0, 1}, // LOW priority
            {0, 1, 0, 3}, // MEDIUM priority
            {0, 1, 0, 5}  // HIGH priority
    },
    // Index 2: POLICE incidents
    {
            {0, 0, 1, 2}, // LOW priority
            {0, 1, 1, 4}, // MEDIUM priority
            {0, 1, 1, 8}  // HIGH priority
    }
};


void dispatch_units() {


// ------------------------------------------------------------DONE: Implement the logic for dispatching units to incidents.
// For each incident, find the appropriate units based on required types and proximity.
// Select the nearest eligible unit and update its state to dispatched.
// Track dispatched units in the incident structure.
// Hint: Use loops and Manhattan distance to determine proximity.



    // Iterate over all incidents
    for (int i = 0; i < incident_count; i++) {
        Incident *incident = &incidents[i];

        // We only dispatch units to incidents that are waiting for them
        if (incident->state != INCIDENT_WAITING) {
            continue; // Skip to the next incident
        }

        // Get the requirements for this specific incident
        const IncidentRequirement *req = &incident_requirements[incident->type][incident->priority];
        int required_counts[3] = {req->fire_units, req->medical_units, req->police_units};

        // For each type of unit (FIRE, MEDICAL, POLICE)
        for (int unit_type = 0; unit_type < 3; unit_type++) {
            // How many units of this type do we still need to dispatch?
            int needed = required_counts[unit_type] - incident->dispatched_count[unit_type];

            for (int k = 0; k < needed; k++) { // Loop to find 'needed' number of units
                Unit *best_unit = NULL;
                int min_dist = 10000; // A large number for initial distance

                // Find the closest available unit of the required type
                for (int d_idx = 0; d_idx < department_count; d_idx++) {
                    for (int u_idx = 0; u_idx < departments[d_idx].unit_count; u_idx++) {
                        Unit *current_unit = &departments[d_idx].units[u_idx];

                        if (current_unit->type == unit_type && current_unit->state == UNIT_WAITING) {
                            int dist = manhattan_distance(current_unit->x, current_unit->y, incident->x, incident->y);
                            if (dist < min_dist) {
                                min_dist = dist;
                                best_unit = current_unit;
                            }
                        }
                    }
                }

                // If we found a suitable unit, dispatch it
                if (best_unit != NULL) {
                    best_unit->state = UNIT_DISPATCHED;
                    best_unit->target_x = incident->x;
                    best_unit->target_y = incident->y;

                    // Store the pointer to the dispatched unit in the incident
                    incident->dispatched_units[unit_type][incident->dispatched_count[unit_type]] = best_unit;
                    incident->dispatched_count[unit_type]++;

                    log_unit_dispatched(best_unit, incident->x, incident->y);
                    log_unit_state_change(best_unit, UNIT_DISPATCHED);
                }
            }
        }
    }


                      
}

void update_incidents() {

// ----------------------------------------------------------------DONE: Implement logic for updating incidents over time.
// For each incident, check if all required units have arrived at the location.
// Update the state of the incident based on unit arrivals and remaining operation time.
// When an incident finishes, update the state of its dispatched units accordingly. 
// Also remember to reset the dispatch count after operation is finished.




    // Iterate over all incidents to update their state
    for (int i = 0; i < incident_count; i++) {
        Incident *incident = &incidents[i];

        // Skip incidents that are already finished
        if (incident->state == INCIDENT_FINISHED) {
            continue;
        }

        // --- Part 1: Check if a WAITING incident is ready to become OPERATIONAL ---
        if (incident->state == INCIDENT_WAITING) {
            const IncidentRequirement *req = &incident_requirements[incident->type][incident->priority];
            int required_counts[3] = {req->fire_units, req->medical_units, req->police_units};

            bool all_units_arrived = true;

            // Check if we have dispatched all the units we need
            for(int type = 0; type < 3; type++) {
                if (incident->dispatched_count[type] < required_counts[type]) {
                    all_units_arrived = false;
                    break;
                }
            }

            // If all units have been dispatched, check if they have arrived (i.e., are OPERATING)
            if (all_units_arrived) {
                for (int type = 0; type < 3; type++) {
                    for (int u = 0; u < incident->dispatched_count[type]; u++) {
                        if (incident->dispatched_units[type][u]->state != UNIT_OPERATING) {
                            all_units_arrived = false;
                            break;
                        }
                    }
                    if (!all_units_arrived) break;
                }
            }

            // If all required units have arrived and are operating, start the incident operation
            if (all_units_arrived) {
                incident->state = INCIDENT_OPERATION;
                log_incident_status_change(incident->x, incident->y, INCIDENT_OPERATION, incident->type);
            }
        }

        // --- Part 2: Update an OPERATIONAL incident ---
        if (incident->state == INCIDENT_OPERATION) {
            incident->operation_turns_remaining--;

            // Check if the operation is now finished
            if (incident->operation_turns_remaining <= 0) {
                incident->state = INCIDENT_FINISHED;
                log_incident_finished(incident->type, incident->x, incident->y);
                log_incident_status_change(incident->x, incident->y, INCIDENT_FINISHED, incident->type);

                // Send all involved units back to their base
                for (int type = 0; type < 3; type++) {
                    for (int u = 0; u < incident->dispatched_count[type]; u++) {
                        Unit *unit = incident->dispatched_units[type][u];
                        unit->state = UNIT_RETURNING;

                        // Find the unit's base location
                        for (int d = 0; d < department_count; d++) {
                            if (departments[d].number == unit->departmentNumber && departments[d].type == unit->type) {
                                unit->target_x = departments[d].x;
                                unit->target_y = departments[d].y;
                                log_unit_returning(unit, unit->target_x, unit->target_y);
                                log_unit_state_change(unit, UNIT_RETURNING);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

}




void ensure_incident_capacity() {
    if (incident_count >= incident_capacity) {
        // -------------------------------------------------------DONE: Implement memory reallocation logic if using dynamic memory for incidents.
        // If using static arrays, explain why this function is unnecessary.


        // If we're out of space, double the capacity
        int new_capacity = (incident_capacity == 0) ? 10 : incident_capacity * 2;
        Incident *new_incidents = realloc(incidents, new_capacity * sizeof(Incident));

        if (new_incidents == NULL) {
            perror("Failed to reallocate memory for incidents");
            exit(1);
        }
        incidents = new_incidents;
        incident_capacity = new_capacity;
    }

}


void create_new_incident(IncidentType type, Priority priority, int x, int y) {
    ensure_incident_capacity();

    // Get the pointer to the next available incident slot
    Incident *new_incident = &incidents[incident_count];

    // Initialize the incident's properties
    new_incident->type = type;
    new_incident->priority = priority;
    new_incident->x = x;
    new_incident->y = y;
    new_incident->state = INCIDENT_WAITING;

    // Set the required operation time from our requirements table
    const IncidentRequirement *req = &incident_requirements[type][priority];
    new_incident->operation_turns_remaining = req->time_required;

    // Reset the dispatched unit counts
    for (int i = 0; i < 3; i++) {
        new_incident->dispatched_count[i] = 0;
    }

    // Log the creation of the new incident
    log_incident_created(type, priority, x, y);

    // Increment the total number of incidents
    incident_count++;
}

