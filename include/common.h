#ifndef COMMON_H_
#define COMMON_H_

#include <time.h>

#define NAME_LEN 64
#define CONTACT_LEN 32
#define DISEASE_LEN 64
#define SPECIALIZATION_LEN 64

typedef struct {
    int patient_id;
    char name[NAME_LEN];
    int age;
    char disease[DISEASE_LEN];
    char contact[CONTACT_LEN];
    int isActive;
} Patient;

typedef struct {
    int doctor_id;
    char name[NAME_LEN];
    char specialization[SPECIALIZATION_LEN];
    char contact[CONTACT_LEN];
    int isActive;
} Doctor;

typedef struct {
    int service_id;
    char name[NAME_LEN];
    float cost;
    int isActive;
} Service;

typedef struct {
    int appointment_id;
    int patient_id;      /* FK -> Patient */
    int doctor_id;       /* FK -> Doctor */
    time_t appointment_date; /* use time_t */
    int status;          /* 1=Scheduled, 0=Cancelled */
} Appointment;

typedef struct {
    int bill_id;
    int patient_id;      /* FK -> Patient */
    float total_amount;
    int status;          /* 1=Unpaid, 2=Paid */
} Bill;

typedef struct {
    int bill_item_id;
    int bill_id;         /* FK -> Bill */
    int service_id;      /* FK -> Service */
} BillItem;

#endif /* COMMON_H_ */
