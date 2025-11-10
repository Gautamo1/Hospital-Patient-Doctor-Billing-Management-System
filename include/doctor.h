#ifndef DOCTOR_H_
#define DOCTOR_H_

#include "common.h"

extern const char *DOCTOR_FILE;

void add_doctor(void);
void list_all_doctors(void);
void search_doctor_by_id(void);
void update_doctor(void);
void delete_doctor(void);

/* Helpers for FK/joins */
int doctor_exists(int id);
int get_doctor_name(int id, char *out, int out_len);

#endif /* DOCTOR_H_ */
