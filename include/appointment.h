#ifndef APPOINTMENT_H_
#define APPOINTMENT_H_

#include "common.h"

extern const char *APPOINTMENT_FILE;

void add_appointment(void);
void list_all_appointments(void);
void cancel_appointment(void);

#endif /* APPOINTMENT_H_ */
