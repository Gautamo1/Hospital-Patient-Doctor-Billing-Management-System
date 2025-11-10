#ifndef SERVICE_H_
#define SERVICE_H_

#include "common.h"

extern const char *SERVICE_FILE;

void add_service(void);
void list_all_services(void);
void update_service(void);
void delete_service(void);
void search_service_by_id(void);

/* Helpers */
int service_exists(int id);
int get_service_cost(int id, float *out_cost);
int get_service_name(int id, char *out, int out_len);

#endif /* SERVICE_H_ */
