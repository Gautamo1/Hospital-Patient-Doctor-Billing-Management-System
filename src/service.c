#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "service.h"
#include "input.h"

const char *SERVICE_FILE = "data/services.dat";

static int next_service_id(void) {
    FILE *fp = fopen(SERVICE_FILE, "rb");
    if (!fp) return 1;
    Service s; int max_id = 0;
    while (fread(&s, sizeof(Service), 1, fp) == 1) {
        if (s.service_id > max_id) max_id = s.service_id;
    }
    fclose(fp);
    return max_id + 1;
}

int service_exists(int id) {
    FILE *fp = fopen(SERVICE_FILE, "rb");
    if (!fp) return 0;
    Service s;
    while (fread(&s, sizeof(Service), 1, fp) == 1) {
        if (s.service_id == id && s.isActive == 1) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int get_service_cost(int id, float *out_cost) {
    if (!out_cost) return 0;
    FILE *fp = fopen(SERVICE_FILE, "rb");
    if (!fp) return 0;
    Service s;
    while (fread(&s, sizeof(Service), 1, fp) == 1) {
        if (s.service_id == id && s.isActive == 1) {
            *out_cost = s.cost;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int get_service_name(int id, char *out, int out_len) {
    if (!out || out_len <= 0) return 0;
    FILE *fp = fopen(SERVICE_FILE, "rb");
    if (!fp) { return 0; }
    Service s;
    while (fread(&s, sizeof(Service), 1, fp) == 1) {
        if (s.service_id == id && s.isActive == 1) {
            strncpy(out, s.name, out_len);
            out[out_len-1] = '\0';
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void add_service(void) {
    Service s;
    s.service_id = next_service_id();
    get_string_input("Enter service name: ", s.name, sizeof(s.name));
    while (1) {
        printf("Enter service cost: ");
        char buf[128]; if (!fgets(buf, sizeof(buf), stdin)) { clearerr(stdin); continue; }
        char *end; float c = strtof(buf, &end);
        if (end == buf || *end != '\n') { printf("Invalid cost. Try again.\n"); continue; }
        s.cost = c; break;
    }
    s.isActive = 1;

    FILE *fp = fopen(SERVICE_FILE, "ab");
    if (!fp) { perror("Failed to open services file"); return; }
    if (fwrite(&s, sizeof(Service), 1, fp) != 1) perror("Failed to write service");
    fclose(fp);
    printf("Service added with ID %d\n", s.service_id);
}

void list_all_services(void) {
    FILE *fp = fopen(SERVICE_FILE, "rb");
    if (!fp) { printf("No services found.\n"); return; }
    Service s; int found = 0;
    printf("\n-- Active Services --\n");
    while (fread(&s, sizeof(Service), 1, fp) == 1) {
        if (s.isActive == 1) {
            printf("ID:%d | Name:%s | Cost:%.2f\n", s.service_id, s.name, s.cost);
            found = 1;
        }
    }
    if (!found) printf("No active services.\n");
    fclose(fp);
}

void update_service(void) {
    int id = get_integer_input("Enter service ID to update: ");
    FILE *fp = fopen(SERVICE_FILE, "rb+");
    if (!fp) { printf("No services file.\n"); return; }
    Service s;
    while (fread(&s, sizeof(Service), 1, fp) == 1) {
        if (s.service_id == id && s.isActive == 1) {
            printf("Updating service '%s'\n", s.name);
            get_string_input("New name: ", s.name, sizeof(s.name));
            while (1) {
                printf("New cost: ");
                char buf[128]; if (!fgets(buf, sizeof(buf), stdin)) { clearerr(stdin); continue; }
                char *end; float c = strtof(buf, &end);
                if (end == buf || *end != '\n') { printf("Invalid cost. Try again.\n"); continue; }
                s.cost = c; break;
            }
            fseek(fp, -((long)sizeof(Service)), SEEK_CUR);
            if (fwrite(&s, sizeof(Service), 1, fp) != 1) perror("Write failed");
            fclose(fp);
            printf("Service updated.\n");
            return;
        }
    }
    printf("Active service with ID %d not found.\n", id);
    fclose(fp);
}

void delete_service(void) {
    int id = get_integer_input("Enter service ID to delete (soft): ");
    FILE *fp = fopen(SERVICE_FILE, "rb+");
    if (!fp) { printf("No services file.\n"); return; }
    Service s;
    while (fread(&s, sizeof(Service), 1, fp) == 1) {
        if (s.service_id == id && s.isActive == 1) {
            s.isActive = 0;
            fseek(fp, -((long)sizeof(Service)), SEEK_CUR);
            if (fwrite(&s, sizeof(Service), 1, fp) != 1) perror("Write failed");
            fclose(fp);
            printf("Service soft-deleted.\n");
            return;
        }
    }
    printf("Active service with ID %d not found.\n", id);
    fclose(fp);
}

void search_service_by_id(void) {
    int id = get_integer_input("Enter service ID to search: ");
    FILE *fp = fopen(SERVICE_FILE, "rb");
    if (!fp) { printf("No services file.\n"); return; }
    Service s; int ok = 0;
    while (fread(&s, sizeof(Service), 1, fp) == 1) {
        if (s.service_id == id && s.isActive == 1) {
            printf("Found: ID:%d | Name:%s | Cost:%.2f\n", s.service_id, s.name, s.cost);
            ok = 1; break;
        }
    }
    if (!ok) printf("Active service with ID %d not found.\n", id);
    fclose(fp);
}
