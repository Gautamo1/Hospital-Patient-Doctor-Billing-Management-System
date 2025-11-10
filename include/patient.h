#ifndef PATIENT_H_
#define PATIENT_H_

#include "common.h"

extern const char *PATIENT_FILE;

void add_patient(void);
void list_all_patients(void);
void search_patient_by_id(void);
void update_patient(void);
void delete_patient(void);

/* Helpers for FK/joins */
int patient_exists(int id);
int get_patient_name(int id, char *out, int out_len);

#endif /* PATIENT_H_ */
