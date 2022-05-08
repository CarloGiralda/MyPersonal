#pragma once
#include "db.h"

enum actions {
	USER,
	CONSULTATION,
    EXPIRE,
    SHOW,
    QUIT,
	END_OF_ACTIONS
};

int get_manager_action(void);
void get_user_inf(struct user *user);
void print_user_id(struct user *user);
void get_cons_inf(struct user *user, struct cons *cons);
void get_cons_code(struct cons *cons);
void get_manager_id(struct user *user);
void print_consultations(struct consultations *consultations);
