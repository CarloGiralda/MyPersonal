#pragma once
#include <stdbool.h>
#include <stdlib.h>

extern bool init_db(void);
extern void fini_db(void);

#define DATE_LEN 11
#define TIME_LEN 6
#define DATETIME_LEN (DATE_LEN + TIME_LEN)

#define USERNAME_LEN 45
#define PASSWORD_LEN 45
struct credentials {
	char username[USERNAME_LEN];
	char password[PASSWORD_LEN];
};

#define NAME_LEN 45
#define SURNAME_LEN 45
#define LOCATION_LEN 128
#define ID_LEN 128
#define NUMBER_LEN 128
#define TEXT_LEN 4096

typedef enum {
	LOGIN_ROLE,
	UTENTE,
    MANAGER,
    COMMERCIALE,
	FAILED_LOGIN
} role_t;

struct user{
    char id[ID_LEN];
    char role[NAME_LEN];
    struct credentials *credentials;
};

struct client{
	char code[ID_LEN];
	char name[NAME_LEN];
	char surname[SURNAME_LEN];
	char birthday[DATE_LEN];
	char registration[DATE_LEN];
	char fax[NUMBER_LEN];
	char email[NUMBER_LEN];
	char address[NUMBER_LEN];
	char phone[NUMBER_LEN];
	char comm[ID_LEN];
	char funz[ID_LEN];
    char IVA[NAME_LEN];
};

struct appointment{
	char day[DATE_LEN];
	char time[TIME_LEN];
	char place[LOCATION_LEN];
	char func[ID_LEN];
	char client[ID_LEN];
    int id;
};

struct appointments{
    unsigned num_rows;
    struct appointment appointment[];
};

struct cons{
	char cod[ID_LEN];
	char day[DATE_LEN];
};

struct consultations{
    unsigned num_rows;
    struct cons consultation[];
};

struct note{
	char number[ID_LEN];
	char text[TEXT_LEN];
	char func[ID_LEN];
	char input[ID_LEN];
};

struct bought{
	char client[ID_LEN];
	char cod[ID_LEN];
};

struct fullclient{
	struct client *client;
	struct note **note;
	struct bought **bought;
};

struct contact{
    char ID[ID_LEN];
    char client[ID_LEN];
    char user[ID_LEN];
};

static void close_prepared_stmts(void);
static bool initialize_prepared_stmts(role_t for_role);
role_t attempt_login(struct credentials *cred);
void db_switch_to_login(void);
void db_switch_to_utente(void);
void do_register_client(struct client *client);
int do_register_appointment(struct appointment *app);
void db_switch_to_manager(void);
void db_switch_to_comm(void);
void do_register_user(struct user *user);
struct appointments *get_agenda(struct user *user);
void dispose_agenda(struct appointments *app);
struct client **get_clients(struct user *user);
void dispose_clients(struct client **clients);
void do_register_consultation(struct user *user, struct cons *cons);
void do_register_opportunity(struct client *client, struct cons *cons);
void do_register_note(struct note *note, struct cons *cons);
void do_modify_note(struct note *note);
void do_delete_note(struct note *note);
struct fullclient *get_full_client(struct client *client);
void dispose_full(struct fullclient *fc);
void expire_consultation(struct cons *cons);
struct consultations *get_active_consultations(struct user *user);
void dispose_cons(struct consultations *cons);
struct contact *do_register_contact(struct contact *contact);
