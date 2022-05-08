#pragma once
#include "db.h"

enum actions {
	AGENDA,
	CLIENT,
	MY_CLIENTS,
	INS_NOTE,
    MOD_NOTE,
    DEL_NOTE,
    OPPORTUNITY,
    APPOINTMENT,
    CONTACT,
    QUIT,
	END_OF_ACTIONS
};


extern int get_user_action(void);
extern void get_user_id(struct user *user);
extern void print_agenda(struct appointments *app);
extern void get_client_id(struct client *client);
extern void print_client(struct fullclient *fc);
extern void show_clients(struct client **clients);
extern void ins_note(struct note *note, struct cons *cons);
extern void mod_note(struct note *note);
extern void del_note(struct note *note);
extern void get_opportunity(struct cons *cons);
extern void get_appointment_information(struct appointment *app);
extern void print_appointment(int cod);
extern void get_contact_inf(struct contact *contact);
extern void print_contact_id(struct contact *contact);
