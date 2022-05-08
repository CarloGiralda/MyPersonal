#include <stdio.h>
#include <string.h>

#include "db.h"
#include "utente.h"
#include "utente_app.h"
#include "io.h"

static bool show_agenda(void)
{
	struct user user;
    struct appointments *app;
	memset(&user, 0, sizeof(user));
	get_user_id(&user);
	app = get_agenda(&user);
    print_agenda(app);
    dispose_agenda(app);
    return false;
}

static bool show_client(void)
{
	struct client client;
	struct fullclient *fc;
	memset(&client, 0, sizeof(client));
	get_client_id(&client);
	fc = get_full_client(&client);
	print_client(fc);
    dispose_full(fc);
	return false;
}

static bool show_my_clients(void)
{
    struct user user;
    struct client **clients;
    memset(&user, 0, sizeof(user));
	get_user_id(&user);
    clients = get_clients(&user);
    show_clients(clients);
    dispose_clients(clients);
    return false;
}

static bool insert_note(void)
{
    struct note note;
    struct cons cons;
    memset(&note, 0, sizeof(note));
    memset(&cons, 0, sizeof(cons));
    ins_note(&note, &cons);
    do_register_note(&note, &cons);
    return false;
}

static bool modify_note(void)
{
    struct note note;
    memset(&note, 0, sizeof(note));
    mod_note(&note);
    do_modify_note(&note);
    return false;
}

static bool delete_note(void)
{
    struct note note;
    memset(&note, 0, sizeof(note));
    del_note(&note);
    do_delete_note(&note);
    return false;
}

static bool offer_opportunity(void)
{
    struct client client;
    struct cons cons;
    memset(&client, 0, sizeof(client));
	get_client_id(&client);
    memset(&cons, 0, sizeof(cons));
    get_opportunity(&cons);
    do_register_opportunity(&client, &cons);
    return false;
}

static bool register_appointment(void)
{
    struct appointment app;
    int cod;
    memset(&app, 0, sizeof(app));
    get_appointment_information(&app);
    cod = do_register_appointment(&app);
    if(cod < 0) {
        printf("Something gone wrong. Try again later\n");
        return true;
    }
    print_appointment(cod);
    return false;
}

static bool register_contact(void)
{
    struct contact cont;
    memset(&cont, 0, sizeof(cont));
    get_contact_inf(&cont);
    do_register_contact(&cont);
    print_contact_id(&cont);
    return false;
}

static bool quit(void)
{
	return true;
}

static struct {
	enum actions action;
	bool (*control)(void);
} controls[END_OF_ACTIONS] = {
	{.action = AGENDA, .control = show_agenda},
	{.action = CLIENT, .control = show_client},
	{.action = MY_CLIENTS, .control = show_my_clients},
	{.action = INS_NOTE, .control = insert_note},
	{.action = MOD_NOTE, .control = modify_note},
	{.action = DEL_NOTE, .control = delete_note},
	{.action = OPPORTUNITY, .control = offer_opportunity},
	{.action = APPOINTMENT, .control = register_appointment},
    {.action = CONTACT, .control = register_contact},
	{.action = QUIT, .control = quit}
};

void controller_utente(void)
{
	db_switch_to_utente();

	while(true) {
		int action = get_user_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}
