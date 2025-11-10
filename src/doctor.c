#include <stdio.h>
#include <string.h>
#include "doctor.h"
#include "input.h"

const char *DOCTOR_FILE = "data/doctors.dat";

static int next_doctor_id(void) {
    FILE *fp = fopen(DOCTOR_FILE, "rb");
    if (!fp) return 1;
    Doctor d; int max_id = 0;
    while (fread(&d, sizeof(Doctor), 1, fp) == 1) {
        if (d.doctor_id > max_id) max_id = d.doctor_id;
    }
    fclose(fp);
    return max_id + 1;
}

int doctor_exists(int id) {
    FILE *fp = fopen(DOCTOR_FILE, "rb");
    if (!fp) return 0;
    Doctor d;
    while (fread(&d, sizeof(Doctor), 1, fp) == 1) {
        if (d.doctor_id == id && d.isActive == 1) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int get_doctor_name(int id, char *out, int out_len) {
    if (!out || out_len <= 0) return 0;
    FILE *fp = fopen(DOCTOR_FILE, "rb");
    if (!fp) { strncpy(out, "N/A", out_len); return 0; }
    Doctor d;
    while (fread(&d, sizeof(Doctor), 1, fp) == 1) {
        if (d.doctor_id == id && d.isActive == 1) {
            strncpy(out, d.name, out_len);
            out[out_len-1] = '\0';
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    strncpy(out, "N/A", out_len);
    return 0;
}

void add_doctor(void) {
    Doctor d;
    d.doctor_id = next_doctor_id();
    get_string_input("Enter name: ", d.name, sizeof(d.name));
    get_string_input("Enter specialization: ", d.specialization, sizeof(d.specialization));
    get_string_input("Enter contact: ", d.contact, sizeof(d.contact));
    d.isActive = 1;

    FILE *fp = fopen(DOCTOR_FILE, "ab");
    if (!fp) { perror("Failed to open doctor file"); return; }
    if (fwrite(&d, sizeof(Doctor), 1, fp) != 1) perror("Failed to write doctor");
    fclose(fp);
    printf("Doctor added with ID %d\n", d.doctor_id);
}

void list_all_doctors(void) {
    FILE *fp = fopen(DOCTOR_FILE, "rb");
    if (!fp) { printf("No doctors found.\n"); return; }
    Doctor d; int found = 0;
    printf("\n-- Active Doctors --\n");
    while (fread(&d, sizeof(Doctor), 1, fp) == 1) {
        if (d.isActive == 1) {
            printf("ID:%d | Name:%s | Spec:%s | Contact:%s\n",
                d.doctor_id, d.name, d.specialization, d.contact);
            found = 1;
        }
    }
    if (!found) printf("No active doctors.\n");
    fclose(fp);
}

void search_doctor_by_id(void) {
    int id = get_integer_input("Enter doctor ID to search: ");
    FILE *fp = fopen(DOCTOR_FILE, "rb");
    if (!fp) { printf("No doctors file.\n"); return; }
    Doctor d; int ok = 0;
    while (fread(&d, sizeof(Doctor), 1, fp) == 1) {
        if (d.doctor_id == id && d.isActive == 1) {
            printf("Found: ID:%d | Name:%s | Spec:%s | Contact:%s\n",
                d.doctor_id, d.name, d.specialization, d.contact);
            ok = 1; break;
        }
    }
    if (!ok) printf("Active doctor with ID %d not found.\n", id);
    fclose(fp);
}

void update_doctor(void) {
    int id = get_integer_input("Enter doctor ID to update: ");
    FILE *fp = fopen(DOCTOR_FILE, "rb+");
    if (!fp) { printf("No doctors file.\n"); return; }
    Doctor d;
    while (fread(&d, sizeof(Doctor), 1, fp) == 1) {
        if (d.doctor_id == id && d.isActive == 1) {
            printf("Updating doctor '%s'\n", d.name);
            get_string_input("New name: ", d.name, sizeof(d.name));
            get_string_input("New specialization: ", d.specialization, sizeof(d.specialization));
            get_string_input("New contact: ", d.contact, sizeof(d.contact));
            fseek(fp, -((long)sizeof(Doctor)), SEEK_CUR);
            if (fwrite(&d, sizeof(Doctor), 1, fp) != 1) perror("Write failed");
            fclose(fp);
            printf("Doctor updated.\n");
            return;
        }
    }
    printf("Active doctor with ID %d not found.\n", id);
    fclose(fp);
}

void delete_doctor(void) {
    int id = get_integer_input("Enter doctor ID to delete (soft): ");
    FILE *fp = fopen(DOCTOR_FILE, "rb+");
    if (!fp) { printf("No doctors file.\n"); return; }
    Doctor d;
    while (fread(&d, sizeof(Doctor), 1, fp) == 1) {
        if (d.doctor_id == id && d.isActive == 1) {
            d.isActive = 0;
            fseek(fp, -((long)sizeof(Doctor)), SEEK_CUR);
            if (fwrite(&d, sizeof(Doctor), 1, fp) != 1) perror("Write failed");
            fclose(fp);
            printf("Doctor soft-deleted.\n");
            return;
        }
    }
    printf("Active doctor with ID %d not found.\n", id);
    fclose(fp);
}
