#ifndef BILLING_H_
#define BILLING_H_

#include "common.h"

extern const char *BILL_FILE;
extern const char *BILL_ITEM_FILE;

void create_new_bill(void);
void view_bill_details(void);
void mark_bill_paid(void);

#endif /* BILLING_H_ */
