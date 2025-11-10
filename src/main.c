#include <stdio.h>
#include "input.h"
#include "patient.h"
#include "doctor.h"
#include "service.h"
#include "appointment.h"
#include "billing.h"

static void patient_menu(void) {
    int choice;
    do {
        printf("\n-- Patient Menu --\n");
        printf("1. Add Patient\n");
        printf("2. List Patients\n");
        printf("3. Search Patient by ID\n");
        printf("4. Update Patient\n");
        printf("5. Delete Patient (Soft)\n");
        printf("0. Back\n");
        choice = get_integer_input("Choice: ");
        switch (choice) {
            case 1: add_patient(); break;
            case 2: list_all_patients(); break;
            case 3: search_patient_by_id(); break;
            case 4: update_patient(); break;
            case 5: delete_patient(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

static void doctor_menu(void) {
    int choice;
    do {
        printf("\n-- Doctor Menu --\n");
        printf("1. Add Doctor\n");
        printf("2. List Doctors\n");
        printf("3. Search Doctor by ID\n");
        printf("4. Update Doctor\n");
        printf("5. Delete Doctor (Soft)\n");
        printf("0. Back\n");
        choice = get_integer_input("Choice: ");
        switch (choice) {
            case 1: add_doctor(); break;
            case 2: list_all_doctors(); break;
            case 3: search_doctor_by_id(); break;
            case 4: update_doctor(); break;
            case 5: delete_doctor(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

static void service_menu(void) {
    int choice;
    do {
        printf("\n-- Service Menu --\n");
        printf("1. Add Service\n");
        printf("2. List Services\n");
        printf("3. Search Service by ID\n");
        printf("4. Update Service\n");
        printf("5. Delete Service (Soft)\n");
        printf("0. Back\n");
        choice = get_integer_input("Choice: ");
        switch (choice) {
            case 1: add_service(); break;
            case 2: list_all_services(); break;
            case 3: search_service_by_id(); break;
            case 4: update_service(); break;
            case 5: delete_service(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

static void appointment_menu(void) {
    int choice;
    do {
        printf("\n-- Appointment Menu --\n");
        printf("1. Add Appointment\n");
        printf("2. List Appointments\n");
        printf("3. Cancel Appointment\n");
        printf("0. Back\n");
        choice = get_integer_input("Choice: ");
        switch (choice) {
            case 1: add_appointment(); break;
            case 2: list_all_appointments(); break;
            case 3: cancel_appointment(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

static void billing_menu(void) {
    int choice;
    do {
        printf("\n-- Billing Menu --\n");
        printf("1. Create New Bill\n");
        printf("2. View Bill Details\n");
        printf("3. Mark Bill as PAID\n");
        printf("0. Back\n");
        choice = get_integer_input("Choice: ");
        switch (choice) {
            case 1: create_new_bill(); break;
            case 2: view_bill_details(); break;
            case 3: mark_bill_paid(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

int main(void) {
    int choice;
    do {
        printf("\n==== Hospital Management System ====\n");
        printf("1. Patients\n");
        printf("2. Doctors\n");
        printf("3. Services\n");
        printf("4. Appointments\n");
        printf("5. Billing\n");
        printf("0. Exit\n");
        choice = get_integer_input("Choice: ");
        switch (choice) {
            case 1: patient_menu(); break;
            case 2: doctor_menu(); break;
            case 3: service_menu(); break;
            case 4: appointment_menu(); break;
            case 5: billing_menu(); break;
            case 0: printf("Bye.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
    return 0;
}
