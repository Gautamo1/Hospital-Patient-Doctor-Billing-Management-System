#include <stdio.h>
#include <string.h>
#include "billing.h"
#include "patient.h"
#include "service.h"
#include "input.h"

const char *BILL_FILE = "data/bills.dat";
const char *BILL_ITEM_FILE = "data/bill_items.dat";

static int next_bill_id(void) {
    FILE *fp = fopen(BILL_FILE, "rb");
    if (!fp) return 1;
    Bill b; int max_id = 0;
    while (fread(&b, sizeof(Bill), 1, fp) == 1) {
        if (b.bill_id > max_id) max_id = b.bill_id;
    }
    fclose(fp);
    return max_id + 1;
}

static int next_bill_item_id(void) {
    FILE *fp = fopen(BILL_ITEM_FILE, "rb");
    if (!fp) return 1;
    BillItem bi; int max_id = 0;
    while (fread(&bi, sizeof(BillItem), 1, fp) == 1) {
        if (bi.bill_item_id > max_id) max_id = bi.bill_item_id;
    }
    fclose(fp);
    return max_id + 1;
}

void create_new_bill(void) {
    Bill b;
    b.bill_id = next_bill_id();
    b.patient_id = get_integer_input("Enter patient ID for bill: ");
    if (!patient_exists(b.patient_id)) {
        printf("Error: patient ID not found or inactive.\n");
        return;
    }
    b.total_amount = 0.0f;
    b.status = 1; /* unpaid */

    /* Save initial bill to get its ID persisted */
    FILE *fp = fopen(BILL_FILE, "ab");
    if (!fp) { perror("Failed to open bills file"); return; }
    if (fwrite(&b, sizeof(Bill), 1, fp) != 1) { perror("Failed to write bill"); fclose(fp); return; }
    fclose(fp);

    printf("Adding services to bill %d (enter 0 to finish):\n", b.bill_id);
    while (1) {
        list_all_services();
        int sid = get_integer_input("Enter service ID (0 to finish): ");
        if (sid == 0) break;
        if (!service_exists(sid)) {
            printf("Invalid or inactive service.\n");
            continue;
        }
        float cost;
        if (!get_service_cost(sid, &cost)) {
            printf("Could not retrieve service cost.\n");
            continue;
        }
        BillItem bi;
        bi.bill_item_id = next_bill_item_id();
        bi.bill_id = b.bill_id;
        bi.service_id = sid;
        FILE *fpi = fopen(BILL_ITEM_FILE, "ab");
        if (!fpi) { perror("Failed to open bill_items file"); return; }
        if (fwrite(&bi, sizeof(BillItem), 1, fpi) != 1) perror("Failed to write bill item");
        fclose(fpi);
        b.total_amount += cost;
        printf("Added service %d (%.2f). Current total: %.2f\n", sid, cost, b.total_amount);
    }

    /* Update bill with final total_amount */
    FILE *fpu = fopen(BILL_FILE, "rb+");
    if (!fpu) { perror("Failed to open bills file for update"); return; }
    Bill tmp;
    while (fread(&tmp, sizeof(Bill), 1, fpu) == 1) {
        if (tmp.bill_id == b.bill_id) {
            tmp.total_amount = b.total_amount;
            fseek(fpu, -((long)sizeof(Bill)), SEEK_CUR);
            if (fwrite(&tmp, sizeof(Bill), 1, fpu) != 1) perror("Bill update failed");
            fclose(fpu);
            printf("Bill %d finalized. Total: %.2f\n", b.bill_id, b.total_amount);
            return;
        }
    }
    fclose(fpu);
    printf("Bill not found to update.\n");
}

void view_bill_details(void) {
    int bid = get_integer_input("Enter bill ID: ");
    FILE *fp = fopen(BILL_FILE, "rb");
    if (!fp) { printf("No bills file.\n"); return; }
    Bill b; int ok = 0;
    while (fread(&b, sizeof(Bill), 1, fp) == 1) {
        if (b.bill_id == bid) { ok = 1; break; }
    }
    fclose(fp);
    if (!ok) { printf("Bill not found.\n"); return; }

    char pname[NAME_LEN];
    get_patient_name(b.patient_id, pname, sizeof(pname));
    printf("\nBill ID: %d | Patient: %s | Status: %s\n", b.bill_id, pname, b.status==2?"Paid":"Unpaid");
    printf("Items:\n");
    printf("%-5s %-30s %-10s\n", "ID", "Service", "Cost");

    FILE *fpi = fopen(BILL_ITEM_FILE, "rb");
    if (!fpi) { printf("No bill_items file.\n"); return; }
    BillItem bi; float total = 0.0f;
    while (fread(&bi, sizeof(BillItem), 1, fpi) == 1) {
        if (bi.bill_id == bid) {
            char sname[NAME_LEN]; float cost=0.0f;
            if (get_service_name(bi.service_id, sname, sizeof(sname)) &&
                get_service_cost(bi.service_id, &cost)) {
                printf("%-5d %-30s %-10.2f\n", bi.bill_item_id, sname, cost);
                total += cost;
            }
        }
    }
    fclose(fpi);
    printf("Recorded total: %.2f | Recomputed: %.2f\n\n", b.total_amount, total);
}

void mark_bill_paid(void) {
    int bid = get_integer_input("Enter bill ID to mark as PAID: ");
    FILE *fp = fopen(BILL_FILE, "rb+");
    if (!fp) { printf("No bills file.\n"); return; }
    Bill b;
    while (fread(&b, sizeof(Bill), 1, fp) == 1) {
        if (b.bill_id == bid) {
            b.status = 2; /* paid */
            fseek(fp, -((long)sizeof(Bill)), SEEK_CUR);
            if (fwrite(&b, sizeof(Bill), 1, fp) != 1) perror("Write failed");
            fclose(fp);
            printf("Bill %d marked as PAID.\n", bid);
            return;
        }
    }
    printf("Bill ID %d not found.\n", bid);
    fclose(fp);
}
