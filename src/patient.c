#include <stdio.h>
#include <string.h>
#include "patient.h"
#include "input.h"

const char *PATIENT_FILE = "data/patients.dat";

static int next_patient_id(void) {
    FILE *fp = fopen(PATIENT_FILE, "rb");
    if (!fp) return 1;
    Patient p; int max_id = 0;
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.patient_id > max_id) max_id = p.patient_id;
    }
    fclose(fp);
    return max_id + 1;
}

int patient_exists(int id) {
    FILE *fp = fopen(PATIENT_FILE, "rb");
    if (!fp) return 0;
    Patient p;
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.patient_id == id && p.isActive == 1) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int get_patient_name(int id, char *out, int out_len) {
    if (!out || out_len <= 0) return 0;
    FILE *fp = fopen(PATIENT_FILE, "rb");
    if (!fp) { strncpy(out, "N/A", out_len); return 0; }
    Patient p;
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.patient_id == id && p.isActive == 1) {
            strncpy(out, p.name, out_len);
            out[out_len-1] = '\0';
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    strncpy(out, "N/A", out_len);
    return 0;
}

void add_patient(void) {
    Patient p;
    p.patient_id = next_patient_id();
    get_string_input("Enter name: ", p.name, sizeof(p.name));
    p.age = get_integer_input("Enter age: ");
    get_string_input("Enter disease: ", p.disease, sizeof(p.disease));
    get_string_input("Enter contact: ", p.contact, sizeof(p.contact));
    p.isActive = 1;

    FILE *fp = fopen(PATIENT_FILE, "ab");
    if (!fp) { perror("Failed to open patient file"); return; }
    if (fwrite(&p, sizeof(Patient), 1, fp) != 1) perror("Failed to write patient");
    fclose(fp);
    printf("Patient added with ID %d\n", p.patient_id);
}

void list_all_patients(void) {
    FILE *fp = fopen(PATIENT_FILE, "rb");
    if (!fp) { printf("No patients found.\n"); return; }
    Patient p; int found = 0;
    printf("\n-- Active Patients --\n");
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.isActive == 1) {
            printf("ID:%d | Name:%s | Age:%d | Disease:%s | Contact:%s\n",
                p.patient_id, p.name, p.age, p.disease, p.contact);
            found = 1;
        }
    }
    if (!found) printf("No active patients.\n");
    fclose(fp);
}

void search_patient_by_id(void) {
    int id = get_integer_input("Enter patient ID to search: ");
    FILE *fp = fopen(PATIENT_FILE, "rb");
    if (!fp) { printf("No patients file.\n"); return; }
    Patient p; int ok = 0;
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.patient_id == id && p.isActive == 1) {
            printf("Found: ID:%d | Name:%s | Age:%d | Disease:%s | Contact:%s\n",
                p.patient_id, p.name, p.age, p.disease, p.contact);
            ok = 1; break;
        }
    }
    if (!ok) printf("Active patient with ID %d not found.\n", id);
    fclose(fp);
}

void update_patient(void) {
    int id = get_integer_input("Enter patient ID to update: ");
    FILE *fp = fopen(PATIENT_FILE, "rb+");
    if (!fp) { printf("No patients file.\n"); return; }
    Patient p;
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.patient_id == id && p.isActive == 1) {
            printf("Updating patient '%s'\n", p.name);
            get_string_input("New name: ", p.name, sizeof(p.name));
            p.age = get_integer_input("New age: ");
            get_string_input("New disease: ", p.disease, sizeof(p.disease));
            get_string_input("New contact: ", p.contact, sizeof(p.contact));
            fseek(fp, -((long)sizeof(Patient)), SEEK_CUR);
            if (fwrite(&p, sizeof(Patient), 1, fp) != 1) perror("Write failed");
            fclose(fp);
            printf("Patient updated.\n");
            return;
        }
    }
    printf("Active patient with ID %d not found.\n", id);
    fclose(fp);
}

void delete_patient(void) {
    int id = get_integer_input("Enter patient ID to delete (soft): ");
    FILE *fp = fopen(PATIENT_FILE, "rb+");
    if (!fp) { printf("No patients file.\n"); return; }
    Patient p;
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.patient_id == id && p.isActive == 1) {
            p.isActive = 0;
            fseek(fp, -((long)sizeof(Patient)), SEEK_CUR);
            if (fwrite(&p, sizeof(Patient), 1, fp) != 1) perror("Write failed");
            fclose(fp);
            printf("Patient soft-deleted.\n");
            return;
        }
    }
    printf("Active patient with ID %d not found.\n", id);
    fclose(fp);
}
