#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "appointment.h"
#include "patient.h"
#include "doctor.h"
#include "input.h"

const char *APPOINTMENT_FILE = "data/appointments.dat";

static int next_appointment_id(void) {
    FILE *fp = fopen(APPOINTMENT_FILE, "rb");
    if (!fp) return 1;
    Appointment a; int max_id = 0;
    while (fread(&a, sizeof(Appointment), 1, fp) == 1) {
        if (a.appointment_id > max_id) max_id = a.appointment_id;
    }
    fclose(fp);
    return max_id + 1;
}

void add_appointment(void) {
    Appointment a;
    a.appointment_id = next_appointment_id();
    a.patient_id = get_integer_input("Enter patient ID: ");
    if (!patient_exists(a.patient_id)) {
        printf("Error: patient ID not found or inactive.\n");
        return;
    }
    a.doctor_id = get_integer_input("Enter doctor ID: ");
    if (!doctor_exists(a.doctor_id)) {
        printf("Error: doctor ID not found or inactive.\n");
        return;
    }
    printf("Enter appointment date as UNIX timestamp (e.g., now=%ld): ", (long)time(NULL));
    char buf[128];
    if (!fgets(buf, sizeof(buf), stdin)) { clearerr(stdin); return; }
    char *end; long t = strtol(buf, &end, 10);
    if (end == buf || (*end != '\0' && *end != '\n')) { printf("Invalid timestamp.\n"); return; }
    a.appointment_date = (time_t)t;
    a.status = 1; /* scheduled */

    FILE *fp = fopen(APPOINTMENT_FILE, "ab");
    if (!fp) { perror("Failed to open appointments file"); return; }
    if (fwrite(&a, sizeof(Appointment), 1, fp) != 1) perror("Failed to write appointment");
    fclose(fp);
    printf("Appointment created with ID %d\n", a.appointment_id);
}

void list_all_appointments(void) {
    FILE *fp = fopen(APPOINTMENT_FILE, "rb");
    if (!fp) { printf("No appointments found.\n"); return; }
    Appointment a; int found = 0;
    char pname[NAME_LEN], dname[NAME_LEN];
    printf("\n-- Appointments --\n");
    while (fread(&a, sizeof(Appointment), 1, fp) == 1) {
        get_patient_name(a.patient_id, pname, sizeof(pname));
        get_doctor_name(a.doctor_id, dname, sizeof(dname));
        char dt[64];
        struct tm *tmv = localtime(&a.appointment_date);
        if (tmv) strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", tmv);
        else snprintf(dt, sizeof(dt), "%ld", (long)a.appointment_date);
        printf("ID:%d | Patient:%s | Doctor:%s | When:%s | Status:%s\n",
            a.appointment_id, pname, dname, dt, a.status==1?"Scheduled":"Cancelled");
        found = 1;
    }
    if (!found) printf("No appointments.\n");
    fclose(fp);
}

void cancel_appointment(void) {
    int id = get_integer_input("Enter appointment ID to cancel: ");
    FILE *fp = fopen(APPOINTMENT_FILE, "rb+");
    if (!fp) { printf("No appointments file.\n"); return; }
    Appointment a;
    while (fread(&a, sizeof(Appointment), 1, fp) == 1) {
        if (a.appointment_id == id) {
            a.status = 0; /* cancelled */
            fseek(fp, -((long)sizeof(Appointment)), SEEK_CUR);
            if (fwrite(&a, sizeof(Appointment), 1, fp) != 1) perror("Write failed");
            fclose(fp);
            printf("Appointment cancelled.\n");
            return;
        }
    }
    printf("Appointment with ID %d not found.\n", id);
    fclose(fp);
}
