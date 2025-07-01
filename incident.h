
#ifndef INCIDENT_H
#define INCIDENT_H

#include "common.h"
#include "unit.h"
#include "stdlib.h"

typedef struct {
    int x, y;
    IncidentType type;
    Priority priority;
    IncidentState state;
    int operation_turns_remaining;
    Unit *dispatched_units[3][MAX_REQUIRED_UNITS_PER_TYPE];
    int dispatched_count[3];
} Incident;


// --------------------------------------------------DONE: Define a struct representing IncidentRequirements. -------------------------------------------
// This type should have number of fire, medical, and police units required for an incident alongside required time.
// Incidents must initiate the  operation_turns_remaining by reading the respective incident requirements


typedef struct {
    int fire_units;
    int medical_units;
    int police_units;
    int time_required; // Operation turns required
} IncidentRequirement;



extern Incident *incidents;
extern int incident_count;
extern int incident_capacity;

// ------------------------------------------------------DONE: Declare the function responsible for dispatching units to incidents.
// ------------------------------------------------------DONE: Declare the function responsible for updating incident states.

// Function declarations
void dispatch_units();
void update_incidents();
void ensure_incident_capacity();
void create_new_incident(IncidentType type, Priority priority, int x, int y);

#endif // INCIDENT_H