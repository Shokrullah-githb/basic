#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "unit.h"
#include "utils.h" // For getting string representations

FILE *log_file = NULL;

// Helper function to print a timestamp to the log file
static void log_timestamp() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);
    fprintf(log_file, "[%s] ", time_str);
}

void init_log() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char filename[128];
    strftime(filename, sizeof(filename), "log_%Y%m%d_%H%M%S.txt", t);
    log_file = fopen(filename, "w");
    if (log_file == NULL) {
        perror("Failed to create log file");
        exit(1);
    }
}

void log_simulation_start() {
    if (!log_file) return;
    log_timestamp();
    fprintf(log_file, "Simulation Started.\n");
    fflush(log_file);
}

void log_simulation_end() {
    if (!log_file) return;
    log_timestamp();
    fprintf(log_file, "Simulation Ended.\n");
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log_incident_created(IncidentType type, Priority priority, int x, int y) {
    if (!log_file) return;
    log_timestamp();
    fprintf(log_file, "Incident Created: Type=%s, Priority=%s, Location=(%d,%d)\n",
            getIncidentTypeStr(type), getPriorityStr(priority), x, y);
    fflush(log_file);
}

void log_unit_dispatched(Unit *u, int target_x, int target_y) {
    if (!log_file) return;
    log_timestamp();
    fprintf(log_file, "Unit Dispatched: Type=%s, Dept=%d, Unit=%d, Target=(%d,%d)\n",
            getUnitTypeStr(u->type), u->departmentNumber, u->unitNumber, target_x, target_y);
    fflush(log_file);
}

void log_incident_finished(IncidentType type, int x, int y) {
    if (!log_file) return;
    log_timestamp();
    fprintf(log_file, "Incident Finished: Type=%s, Location=(%d,%d)\n",
            getIncidentTypeStr(type), x, y);
    fflush(log_file);
}

void log_unit_state_change(Unit *u, UnitState state) {
    if (!log_file) return;
    log_timestamp();
    fprintf(log_file, "Unit State Change: Dept=%d, Unit=%d changed state to %s\n",
            u->departmentNumber, u->unitNumber, getUnitStateStr(state));
    fflush(log_file);
}

void log_unit_arrived(Unit *u, int incident_x, int incident_y) {
    if (!log_file) return;
    log_timestamp();
    fprintf(log_file, "Unit Arrived: Dept=%d, Unit=%d arrived at Incident (%d,%d)\n",
            u->departmentNumber, u->unitNumber, incident_x, incident_y);
    fflush(log_file);
}

void log_unit_returning(Unit *u, int base_x, int base_y) {
    if (!log_file) return;
    log_timestamp();
    fprintf(log_file, "Unit Returning: Dept=%d, Unit=%d returning to base (%d,%d)\n",
            u->departmentNumber, u->unitNumber, base_x, base_y);
    fflush(log_file);
}

void log_incident_status_change(int x, int y, IncidentState state, IncidentType type) {
    if (!log_file) return;
    log_timestamp();
    fprintf(log_file, "Incident Status Change: Incident %s at (%d,%d) changed state to %s\n",
            getIncidentTypeStr(type), x, y, getIncidentStateStr(state));
    fflush(log_file);
}