#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <assert.h>

#include "db.h"
#include "db_prof.h"

static MYSQL *conn;

static MYSQL_STMT *login_procedure;
static MYSQL_STMT *agenda;
static MYSQL_STMT *client_data;
static MYSQL_STMT *client_notes;
static MYSQL_STMT *client_bought;
static MYSQL_STMT *clients_bound;;
static MYSQL_STMT *ins_note;
static MYSQL_STMT *mod_note;
static MYSQL_STMT *del_note;
static MYSQL_STMT *opportunity;
static MYSQL_STMT *reg_appointment;
static MYSQL_STMT *users;
static MYSQL_STMT *consultation;
static MYSQL_STMT *expire;
static MYSQL_STMT *get_consultation;
static MYSQL_STMT *clients;
static MYSQL_STMT *reg_contact;

static void close_prepared_stmts(void)
{
	if(login_procedure) {
		mysql_stmt_close(login_procedure);
		login_procedure = NULL;
	}
	if(agenda) {
		mysql_stmt_close(agenda);
		agenda = NULL;
	}
	if(client_data) {
		mysql_stmt_close(client_data);
		client_data = NULL;
	}
    if(client_notes) {
		mysql_stmt_close(client_notes);
		client_notes = NULL;
	}
    if(client_bought) {
		mysql_stmt_close(client_bought);
		client_bought = NULL;
	}
	if(clients_bound) {
		mysql_stmt_close(clients_bound);
		clients_bound = NULL;
	}
	if(ins_note) {
		mysql_stmt_close(ins_note);
		ins_note = NULL;
	}
	if(mod_note) {
		mysql_stmt_close(mod_note);
		mod_note = NULL;
	}
	if(del_note) {
		mysql_stmt_close(del_note);
		del_note = NULL;
	}
	if(opportunity) {
        mysql_stmt_close(opportunity);
        opportunity = NULL;
    }
    if(reg_appointment) {
        mysql_stmt_close(reg_appointment);
        reg_appointment = NULL;
    }
    if(users) {
        mysql_stmt_close(users);
        users = NULL;
    }
    if(consultation) {
        mysql_stmt_close(consultation);
        consultation = NULL;
    }
    if(expire) {
        mysql_stmt_close(expire);
        expire = NULL;
    }
    if(get_consultation) {
        mysql_stmt_close(get_consultation);
        get_consultation = NULL;
    }
    if(clients) {
        mysql_stmt_close(clients);
        clients = NULL;
    }
    if(reg_contact) {
        mysql_stmt_close(reg_contact);
        reg_contact = NULL;
    }
}

static bool initialize_prepared_stmts(role_t for_role)
{
	switch(for_role) {

		case LOGIN_ROLE:
			if(!setup_prepared_stmt(&login_procedure, "call login(?, ?, ?)", conn)) {
				print_stmt_error(login_procedure, "Unable to initialize login statement\n");
				return false;
			}
			break;
		case UTENTE:
			if(!setup_prepared_stmt(&agenda, "call agenda(?)", conn)) {
				print_stmt_error(agenda, "Unable to initialize agenda statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&client_data, "call visualizzare_cliente_dati(?)", conn)) {
				print_stmt_error(client_data, "Unable to initialize get client statement\n");
				return false;
			}
            if(!setup_prepared_stmt(&client_notes, "call visualizzare_cliente_note(?)", conn)) {
				print_stmt_error(client_notes, "Unable to initialize get client statement\n");
				return false;
			}
            if(!setup_prepared_stmt(&client_bought, "call visualizzare_cliente_acquisti(?)", conn)) {
				print_stmt_error(client_bought, "Unable to initialize get client statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&clients_bound, "call visualizzare_clienti_assegnati(?)", conn)) {
                print_stmt_error(clients_bound, "Unable to initialize get user's clients statement\n");
                return false;
            }
			if(!setup_prepared_stmt(&ins_note, "call inserire_nota(?, ?, ?, ?, ?)", conn)) {
                print_stmt_error(ins_note, "Unable to initialize notes statement\n");
                return false;
            }
			if(!setup_prepared_stmt(&mod_note, "call modificare_nota(?, ?)", conn)) {
                print_stmt_error(mod_note, "Unable to initialize notes statement\n");
                return false;
            }
			if(!setup_prepared_stmt(&del_note, "call eliminare_nota(?)", conn)) {
                print_stmt_error(del_note, "Unable to initialize notes statement\n");
                return false;
            }
			if(!setup_prepared_stmt(&opportunity, "call inserire_opportunità(?, ?)", conn)) {
                print_stmt_error(opportunity, "Unable to initialize new opportunities statement\n");
                return false;
            }
            if(!setup_prepared_stmt(&reg_appointment, "call inserire_appuntamento(?, ?, ?, ?, ?, ?)", conn)) {
                print_stmt_error(reg_appointment, "Unable to initialize new appointments statement\n");
                return false;
            }
            if(!setup_prepared_stmt(&reg_contact, "call inserire_contatto(?, ?, ?)", conn)) {
                print_stmt_error(reg_contact, "Unable to initialize new appointments statement\n");
                return false;
            }
            break;
		case MANAGER:
			if(!setup_prepared_stmt(&users, "call inserire_utente(?, ?, ?, ?)", conn)) {
				print_stmt_error(users, "Unable to initialize new users statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&consultation, "call inserire_consulenza(?, ?, ?)", conn)) {
				print_stmt_error(consultation, "Unable to initialize new consultations statement\n");
				return false;
			}
            if(!setup_prepared_stmt(&expire, "call invalidare_consulenza(?)", conn)) {
				print_stmt_error(expire, "Unable to initialize new consultations statement\n");
				return false;
			}
			if(!setup_prepared_stmt(&get_consultation, "call visualizzare_consulenze_attive(?)", conn)) {
				print_stmt_error(get_consultation, "Unable to initialize new consultations statement\n");
				return false;
			}
			break;
		case COMMERCIALE:
            if(!setup_prepared_stmt(&clients, "call inserire_cliente(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
                print_stmt_error(clients, "Unable to initialize new clients statement\n");
                return false;
            }
            break;
		default:
			fprintf(stderr, "[FATAL] Unexpected role to prepare statements.\n");
			exit(EXIT_FAILURE);
	}

	return true;
}

bool init_db(void)
{
	unsigned int timeout = 300;
	bool reconnect = true;

	conn = mysql_init(NULL);
	if(conn == NULL) {
		finish_with_error(conn, "mysql_init() failed (probably out of memory)\n");
	}

	if(mysql_real_connect(conn, "localhost", "login", "login", "azienda",
			      3306, NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}

	if (mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout)) {
		print_error(conn, "[mysql_options] failed.");
	}
	if(mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect)) {
		print_error(conn, "[mysql_options] failed.");
	}
#ifndef NDEBUG
	mysql_debug("d:t:O,/tmp/client.trace");
	if(mysql_dump_debug_info(conn)) {
		print_error(conn, "[debug_info] failed.");
	}
#endif

	return initialize_prepared_stmts(LOGIN_ROLE);
}


void fini_db(void)
{
	close_prepared_stmts();

	mysql_close(conn);
}


role_t attempt_login(struct credentials *cred)
{
	MYSQL_BIND param[3]; // Used both for input and output
	int role = 0;

	// Prepare parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cred->username, strlen(cred->username));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, cred->password, strlen(cred->password));
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &role, sizeof(role));

	if(mysql_stmt_bind_param(login_procedure, param) != 0) { // Note _param
		print_stmt_error(login_procedure, "Could not bind parameters for login");
		role = FAILED_LOGIN;
		goto out;
	}

	// Run procedure
	if(mysql_stmt_execute(login_procedure) != 0) {
		print_stmt_error(login_procedure, "Could not execute login procedure");
		role = FAILED_LOGIN;
		goto out;
	}

	// Prepare output parameters
	set_binding_param(&param[0], MYSQL_TYPE_LONG, &role, sizeof(role));

	if(mysql_stmt_bind_result(login_procedure, param)) {
		print_stmt_error(login_procedure, "Could not retrieve output parameter");
		role = FAILED_LOGIN;
		goto out;
	}

	// Retrieve output parameter
	if(mysql_stmt_fetch(login_procedure)) {
		print_stmt_error(login_procedure, "Could not buffer results");
		role = FAILED_LOGIN;
		goto out;
	}

    out:
	// Consume the possibly-returned table for the output parameter
//	while(mysql_stmt_next_result(login_procedure) != -1) {}

	mysql_stmt_free_result(login_procedure);
	mysql_stmt_reset(login_procedure);
	return role;
}

void db_switch_to_login(void)
{
	close_prepared_stmts();

    	mysql_close(conn);
    	conn = mysql_init(NULL);
    	if(mysql_real_connect(conn, getenv("HOST"), getenv("LOGIN_USER"), getenv("LOGIN_PASS"), getenv("DB"),
			      3306, NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}


	if(!initialize_prepared_stmts(LOGIN_ROLE)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
		exit(EXIT_FAILURE);
	}
}


void db_switch_to_utente(void)
{
	close_prepared_stmts();

    	mysql_close(conn);
    	conn = mysql_init(NULL);
    	if(mysql_real_connect(conn, getenv("HOST"), getenv("FUNC_USER"), getenv("FUNC_PASS"), getenv("DB"),
			      3306, NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}


	if(!initialize_prepared_stmts(UTENTE)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
		exit(EXIT_FAILURE);
	}
}


void do_register_client(struct client *client)
{
	MYSQL_BIND param[12];
	MYSQL_TIME giornonasc;
	MYSQL_TIME giornoreg;
    int fax, phone, comm, funz;

	date_to_mysql_time(client->birthday, &giornonasc);
	date_to_mysql_time(client->registration, &giornoreg);
    fax = atoi(client->fax);
    phone = atoi(client->phone);
    comm = atoi(client->comm);
    funz = atoi(client->funz);

	// Bind parameters
	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, client->code, sizeof(client->code));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, client->name, sizeof(client->name));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, client->surname, sizeof(client->surname));
	set_binding_param(&param[3], MYSQL_TYPE_TIME, &giornonasc, sizeof(giornonasc));
	set_binding_param(&param[4], MYSQL_TYPE_TIME, &giornoreg, sizeof(giornoreg));
	set_binding_param(&param[5], MYSQL_TYPE_LONG, &fax, sizeof(fax));
	set_binding_param(&param[6], MYSQL_TYPE_VAR_STRING, client->email, sizeof(client->email));
	set_binding_param(&param[7], MYSQL_TYPE_VAR_STRING, client->address, sizeof(client->address));
	set_binding_param(&param[8], MYSQL_TYPE_LONG, &phone, sizeof(phone));
	set_binding_param(&param[9], MYSQL_TYPE_LONG, &comm, sizeof(comm));
	set_binding_param(&param[10], MYSQL_TYPE_LONG, &funz, sizeof(funz));
    set_binding_param(&param[11], MYSQL_TYPE_VAR_STRING, client->IVA, sizeof(client->IVA));

	if(mysql_stmt_bind_param(clients, param) != 0) {
		print_stmt_error(clients, "Could not bind parameters for do_register_client");
		return;
	}

	// Run procedure
	if(mysql_stmt_execute(clients) != 0) {
		print_stmt_error(clients, "Could not execute register clients procedure");
		return;
	}

	mysql_stmt_free_result(clients);
	mysql_stmt_reset(clients);
	return;
}


int do_register_appointment(struct appointment *app)
{
	MYSQL_BIND param[6];
	MYSQL_TIME day;
	MYSQL_TIME time;
    int id;
    int ret;
    int status;

	date_to_mysql_time(app->day, &day);
	time_to_mysql_time(app->time, &time);
    id = atoi(app->func);

	set_binding_param(&param[0], MYSQL_TYPE_DATE, &day, sizeof(day));
	set_binding_param(&param[1], MYSQL_TYPE_TIME, &time, sizeof(time));
	set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, app->place, sizeof(app->place));
	set_binding_param(&param[3], MYSQL_TYPE_LONG, &id, sizeof(id));
	set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, app->client, sizeof(app->client));
    set_binding_param(&param[5], MYSQL_TYPE_LONG, &ret, sizeof(ret));

	if(mysql_stmt_bind_param(reg_appointment, param) != 0) {
		print_stmt_error(reg_appointment, "Could not bind parameters for do_register_appointment");
		ret = -1;
        goto out;
	}

	if(mysql_stmt_execute(reg_appointment) != 0){
		print_stmt_error(reg_appointment, "Could not execute register appointments procedure");
		ret = -1;
        goto out;
	}

    set_binding_param(&param[0], MYSQL_TYPE_LONG, &id, sizeof(id));

    if(mysql_stmt_bind_result(reg_appointment, param)) {
        print_stmt_error(reg_appointment, "Could not bind output parameters");
        ret = -1;
        goto out;
    }

    if (mysql_stmt_store_result(reg_appointment)) {
		print_stmt_error(reg_appointment, "Unable to store appointment information result set.");
        ret = -1;
		goto out;
	}

	do{
        while (true) {
            status = mysql_stmt_fetch(reg_appointment);

            if (status == 1 || status == MYSQL_NO_DATA)
                    break;

            ret = id;
        }
    }while(mysql_stmt_next_result(reg_appointment) == 0);

out:
	mysql_stmt_free_result(reg_appointment);
	mysql_stmt_reset(reg_appointment);

    return ret;
}


void db_switch_to_manager(void)
{
	close_prepared_stmts();

    	mysql_close(conn);
    	conn = mysql_init(NULL);
    	if(mysql_real_connect(conn, getenv("HOST"), getenv("MANAGER_USER"), getenv("MANAGER_PASS"), getenv("DB"),
                  3306, NULL,
			      CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
		finish_with_error(conn, "mysql_real_connect() failed\n");
	}

	if(!initialize_prepared_stmts(MANAGER)) {
		fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
		exit(EXIT_FAILURE);
	}
}

void db_switch_to_comm(void)
{
        close_prepared_stmts();

        mysql_close(conn);
        conn = mysql_init(NULL);
        if(mysql_real_connect(conn, getenv("HOST"), getenv("COMMERCIALE_USER"), getenv("COMMERCIALE_PASS"), getenv("DB"),
                              3306, NULL,
                              CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_COMPRESS | CLIENT_INTERACTIVE | CLIENT_REMEMBER_OPTIONS) == NULL) {
                finish_with_error(conn, "mysql_real_connect() failed\n");
        }

        if(!initialize_prepared_stmts(COMMERCIALE)) {
                fprintf(stderr, "[FATAL] Cannot initialize prepared statements.\n");
                exit(EXIT_FAILURE);
        }
}


struct appointments *get_agenda(struct user *user)
{
    MYSQL_BIND param[1];
    MYSQL_BIND ret[3];
    MYSQL_TIME giorno;
    MYSQL_TIME ora;
    char indirizzo[LOCATION_LEN];
    int row = 0;
    int status;
    int temp;
    struct appointments *appointments = NULL;

    temp = atoi(user->id);

    set_binding_param(&param[0], MYSQL_TYPE_LONG, &temp, sizeof(temp));

	if(mysql_stmt_bind_param(agenda, param) != 0){
		print_stmt_error(agenda, "Could not bind input parameters");
		goto out;
	}

	if(mysql_stmt_execute(agenda) != 0){
		print_stmt_error(agenda, "Could not execute agenda procedure");
		goto out;
	}

	mysql_stmt_store_result(agenda);

    appointments = malloc(sizeof(*appointments) + sizeof(unsigned) + sizeof(struct appointment) * mysql_stmt_num_rows(agenda));
	if(appointments == NULL)
		goto out;
	memset(appointments, 0, sizeof(*appointments) + sizeof(unsigned) + sizeof(struct appointment) * mysql_stmt_num_rows(agenda));

    appointments->num_rows = mysql_stmt_num_rows(agenda);

	set_binding_param(&ret[0], MYSQL_TYPE_DATE, &giorno, sizeof(giorno));
    set_binding_param(&ret[1], MYSQL_TYPE_TIME, &ora, sizeof(ora));
    set_binding_param(&ret[2], MYSQL_TYPE_VAR_STRING, indirizzo, sizeof(indirizzo));

    if(mysql_stmt_bind_result(agenda, ret)) {
        print_stmt_error(agenda, "Unable to bind output parameters");
        goto out;
    }

    while (1) {
        status = mysql_stmt_fetch(agenda);

        if (status == 1 || status == MYSQL_NO_DATA)
                break;

        mysql_date_to_string(&giorno, appointments->appointment[row].day);
        mysql_time_to_string(&ora, appointments->appointment[row].time);
        strcpy(appointments->appointment[row].place, indirizzo);

        row++;
    }

out:
    while(mysql_stmt_next_result(agenda) != -1) {}

	mysql_stmt_free_result(agenda);
	mysql_stmt_reset(agenda);
	return appointments;
}


void dispose_agenda(struct appointments *app)
{
    free(app);
}


struct client **get_clients(struct user *user)
{
	MYSQL_BIND param[1];
	struct client **clients;
	int status, i, temp;
	int row = 0;
	char cod[ID_LEN];

    temp = atoi(user->id);

	set_binding_param(&param[0], MYSQL_TYPE_LONG, &temp, sizeof(temp));

	if(mysql_stmt_bind_param(clients_bound, param) != 0){
		print_stmt_error(clients_bound, "Could not bind input parameters");
		clients = NULL;
		goto out;
	}

	if(mysql_stmt_execute(clients_bound) != 0){
		print_stmt_error(clients_bound, "Could not execute clients bound procedure");
		clients = NULL;
		goto out;
	}

	if (mysql_stmt_store_result(clients_bound)) {
		print_stmt_error(clients_bound, "Unable to store clients information result set.");
		free(clients);
		clients = NULL;
		goto out;
	}

	clients = malloc(mysql_stmt_num_rows(clients_bound) * sizeof(struct client*));
	if(clients == NULL)
		goto out;
	memset(clients, 0, sizeof(struct client*) * mysql_stmt_num_rows(clients_bound));

    for(i = 0; i < mysql_stmt_num_rows(clients_bound); i++) {
        clients[i] = malloc(sizeof(struct client));
        memset(clients[i], 0, sizeof(struct client));
    }

    clients[i] = NULL;

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cod, sizeof(cod));

	if(mysql_stmt_bind_result(clients_bound, param)){
		print_stmt_error(clients_bound, "Unable to bind output parameters");
		free(clients);
		clients = NULL;
		goto out;
	}

	while(true){
		status = mysql_stmt_fetch(clients_bound);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy(clients[row]->code, cod);

		row++;
	}

out:
    while(mysql_stmt_next_result(clients_bound) != -1) {}

	mysql_stmt_free_result(clients_bound);
	mysql_stmt_reset(clients_bound);
	return clients;
}


void dispose_clients(struct client **clients)
{
    free(clients);
}


void do_register_consultation(struct user *user, struct cons *cons)
{
	MYSQL_BIND param[3];
	MYSQL_TIME day;

	date_to_mysql_time(cons->day, &day);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, user->credentials->username, sizeof(user->credentials->username));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, cons->cod, sizeof(cons->cod));
	set_binding_param(&param[2], MYSQL_TYPE_DATE, &day, sizeof(day));

	if(mysql_stmt_bind_param(consultation, param) != 0) {
		print_stmt_error(consultation, "Unable to bind input parameters");
		return;
	}

	if(mysql_stmt_execute(consultation) != 0){
		print_stmt_error(consultation, "Unable to execute consultation registration procedure");
		return;
	}

	mysql_stmt_free_result(consultation);
    mysql_stmt_reset(consultation);
    return;
}


void do_register_opportunity(struct client *client, struct cons *cons)
{
	MYSQL_BIND param[2];

	set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, client->code, sizeof(client->code));
	set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, cons->cod, sizeof(cons->cod));

	if (mysql_stmt_bind_param(opportunity, param) != 0) {
		print_stmt_error(opportunity, "Unable to bind input parameters");
		return;
	}

	if (mysql_stmt_execute(opportunity) != 0) {
		print_stmt_error(opportunity, "An error occurred while adding opportunity to a client.");
		return;
	}

	mysql_stmt_free_result(opportunity);
	mysql_stmt_reset(opportunity);
	return;
}


void do_register_note(struct note *note, struct cons *cons)
{
    MYSQL_BIND param[5];
    int nb, fc, ca;

    nb = atoi(note->number);
    fc = atoi(note->func);
    ca = atoi(note->input);

    set_binding_param(&param[0], MYSQL_TYPE_LONG, &nb, sizeof(nb));
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, note->text, sizeof(note->text));
	set_binding_param(&param[2], MYSQL_TYPE_LONG, &fc, sizeof(fc));
	set_binding_param(&param[3], MYSQL_TYPE_LONG, &ca, sizeof(ca));
    set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, cons->cod, sizeof(cons->cod));

    if (mysql_stmt_bind_param(ins_note, param) != 0) {
        print_stmt_error(ins_note, "Unable to bind input parameters");
        return;
    }

    if (mysql_stmt_execute(ins_note) != 0) {
        print_stmt_error(ins_note, "An error occurred while registering a note");
        return;
    }

    mysql_stmt_free_result(ins_note);
    mysql_stmt_reset(ins_note);
    return;
}


void do_modify_note(struct note *note)
{
        MYSQL_BIND param[2];
        int nb;

        nb = atoi(note->number);

        set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, note->text, sizeof(note->text));
        set_binding_param(&param[1], MYSQL_TYPE_LONG, &nb, sizeof(nb));

        if (mysql_stmt_bind_param(mod_note, param) != 0) {
                print_stmt_error(ins_note, "Unable to bind input parameters");
                return;
        }

        if (mysql_stmt_execute(mod_note) != 0) {
                print_stmt_error(ins_note, "An error occurred while modifying a note.");
                return;
        }

        mysql_stmt_free_result(mod_note);
        mysql_stmt_reset(mod_note);
        return;
}


void do_delete_note(struct note *note)
{
        MYSQL_BIND param[1];
        int nb;

        nb = atoi(note->number);

        set_binding_param(&param[0], MYSQL_TYPE_LONG, &nb, sizeof(nb));

        if (mysql_stmt_bind_param(del_note, param) != 0) {
                print_stmt_error(del_note, "Unable to bind input parameters");
                return;
        }

        if (mysql_stmt_execute(del_note) != 0) {
                print_stmt_error(del_note, "An error occurred while adding opportunity to a client.");
                return;
        }

        mysql_stmt_free_result(del_note);
        mysql_stmt_reset(del_note);
        return;
}


struct client *get_single_client(char *cod)
{
    MYSQL_BIND param[1];
	int status;
	MYSQL_BIND ret[12];

	struct client *cl = NULL;
	char CodiceFiscale[128];
	char Nome[45];
	char Cognome[45];
	MYSQL_TIME Data_di_nascita;
	MYSQL_TIME Data_di_registrazione;
	int Fax;
	char Email[45];
	char Indirizzo[45];
	int Recapito_telefonico;
	int IDComm;
	int IDFunz;
    char IVA[NAME_LEN];

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cod, sizeof(cod));

    if (mysql_stmt_bind_param(client_data, param) != 0) {
        print_stmt_error(client_data, "Unable to bind input parameters");
        cl = NULL;
        goto out;
    }

    if (mysql_stmt_execute(client_data) != 0) {
        print_stmt_error(client_data, "An error occurred while showing a client.");
        cl = NULL;
        goto out;
    }

    set_binding_param(&ret[0], MYSQL_TYPE_VAR_STRING, CodiceFiscale, sizeof(CodiceFiscale));
	set_binding_param(&ret[1], MYSQL_TYPE_VAR_STRING, Nome, sizeof(Nome));
	set_binding_param(&ret[2], MYSQL_TYPE_VAR_STRING, Cognome, sizeof(Cognome));
	set_binding_param(&ret[3], MYSQL_TYPE_DATE, &Data_di_nascita, sizeof(Data_di_nascita));
	set_binding_param(&ret[4], MYSQL_TYPE_DATE, &Data_di_registrazione, sizeof(Data_di_registrazione));
	set_binding_param(&ret[5], MYSQL_TYPE_LONG, &Fax, sizeof(Fax));
    set_binding_param(&ret[6], MYSQL_TYPE_VAR_STRING, Email, sizeof(Email));
    set_binding_param(&ret[7], MYSQL_TYPE_VAR_STRING, Indirizzo, sizeof(Indirizzo));
    set_binding_param(&ret[8], MYSQL_TYPE_LONG, &Recapito_telefonico, sizeof(Recapito_telefonico));
    set_binding_param(&ret[9], MYSQL_TYPE_LONG, &IDComm, sizeof(IDComm));
	set_binding_param(&ret[10], MYSQL_TYPE_LONG, &IDFunz, sizeof(IDFunz));
    set_binding_param(&ret[11], MYSQL_TYPE_VAR_STRING, IVA, sizeof(IVA));

    cl = malloc(sizeof(*cl));
    if(cl == NULL)
        goto out;
    memset(cl, 0, sizeof(*cl));

    if(mysql_stmt_bind_result(client_data, ret)) {
        print_stmt_error(client_data, "Unable to bind output parameters");
        cl = NULL;
        goto out;
    }

    while (1) {
        status = mysql_stmt_fetch(client_data);

        if (status == 1 || status == MYSQL_NO_DATA)
                break;

        strcpy(cl->code, CodiceFiscale);
        strcpy(cl->name, Nome);
        strcpy(cl->surname, Cognome);
        mysql_date_to_string(&Data_di_nascita, cl->birthday);
        mysql_date_to_string(&Data_di_registrazione, cl->registration);
        snprintf(cl->fax, NUMBER_LEN, "%d", Fax);
        strcpy(cl->email, Email);
        strcpy(cl->address, Indirizzo);
        snprintf(cl->phone, NUMBER_LEN, "%d", Recapito_telefonico);
        snprintf(cl->comm, ID_LEN, "%d", IDComm);
        snprintf(cl->funz, ID_LEN, "%d", IDFunz);
        strcpy(cl->IVA, IVA);
    }
out:
    while(mysql_stmt_next_result(client_data) != -1) {}

    mysql_stmt_free_result(client_data);
    mysql_stmt_reset(client_data);

    return cl;
}


struct note **get_client_note(char *cod)
{
    MYSQL_BIND param[1];
    int status, temp, i;

    struct note **nt = NULL;
    int Numero;

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cod, sizeof(cod));

    if (mysql_stmt_bind_param(client_notes, param) != 0) {
        print_stmt_error(client_notes, "Unable to bind input parameters");
        goto out;
    }

    if (mysql_stmt_execute(client_notes) != 0) {
        print_stmt_error(client_notes, "An error occurred while showing client's notes.");
        goto out;
    }

    if (mysql_stmt_store_result(client_notes)) {
		print_stmt_error(client_notes, "Unable to store client notes result set.");
		goto out;
	}

    nt = malloc(sizeof(struct note*) * mysql_stmt_num_rows(client_notes));
    if(nt == NULL)
        goto out;

    for(i = 0; i < mysql_stmt_num_rows(client_notes); i++) {
        nt[i] = malloc(sizeof(struct note));
        memset(nt[i], 0, sizeof(struct note));
    }

    nt[i] = NULL;

    set_binding_param(&param[0], MYSQL_TYPE_LONG, &Numero, sizeof(Numero));

    if(mysql_stmt_bind_result(client_notes, param)) {
        print_stmt_error(client_notes, "Unable to bind output parameters");
        nt = NULL;
        goto out;
    }

    temp = 0;

    while (1) {
        status = mysql_stmt_fetch(client_notes);

        if (status == 1 || status == MYSQL_NO_DATA)
                break;

		snprintf(nt[temp]->number, ID_LEN, "%d", Numero);

        temp++;
	}
out:
    while(mysql_stmt_next_result(client_notes) != -1) {}

	mysql_stmt_free_result(client_notes);
    mysql_stmt_reset(client_notes);

	return nt;
}


struct bought **get_client_bought(char *cod)
{
    MYSQL_BIND param[1];
    int status, temp, i;

    struct bought **bt = NULL;
    char Codice[128];

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cod, sizeof(cod));

    if (mysql_stmt_bind_param(client_bought, param) != 0) {
        print_stmt_error(client_bought, "Unable to bind input parameters");
        goto out;
    }

    if (mysql_stmt_execute(client_bought) != 0) {
        print_stmt_error(client_bought, "An error occurred while showing client's boughts.");
        goto out;
    }

    if (mysql_stmt_store_result(client_bought)) {
		print_stmt_error(client_bought, "Unable to store client boughts result set.");
		goto out;
	}

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, Codice, sizeof(Codice));

    bt = malloc(sizeof(struct bought*) * mysql_stmt_num_rows(client_bought));
    if(bt == NULL)
       goto out;
    for(i = 0; i < mysql_stmt_num_rows(client_bought); i++) {
        bt[i] = malloc(sizeof(struct bought));
        memset(bt[i], 0, sizeof(struct bought));
    }

    bt[i] = NULL;

    if(mysql_stmt_bind_result(client_bought, param)) {
        print_stmt_error(client_bought, "Unable to bind output parameters");
        bt = NULL;
        goto out;
    }

    temp = 0;

    while (1) {
        status = mysql_stmt_fetch(client_bought);

        if (status == 1 || status == MYSQL_NO_DATA)
                break;

        strcpy(bt[temp]->cod, Codice);

        temp++;
    }
out:
    while(mysql_stmt_next_result(client_bought) != -1) {}

    mysql_stmt_free_result(client_bought);
    mysql_stmt_reset(client_bought);

    return bt;
}

struct fullclient *get_full_client(struct client *client)
{
    struct fullclient *fc;
    struct client *cl;
    struct note **note;
    struct bought **bought;

    cl = get_single_client(client->code);
    note = get_client_note(client->code);
    bought = get_client_bought(client->code);

    fc = malloc(sizeof(struct client*) + sizeof(struct note*) + sizeof(struct bought*));
    if(fc == NULL)
        return NULL;
    memset(fc, 0, sizeof(struct client*) + sizeof(struct note*) + sizeof(struct bought*));

	fc->client = cl;
    fc->note = note;
    fc->bought = bought;

    return fc;
}

void dispose_full(struct fullclient *fc)
{
    free(fc);
}


void do_register_user(struct user *user)
{
    MYSQL_BIND param[4];
    int id = 0, status;

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, user->credentials->username, sizeof(user->credentials->username));
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, user->credentials->password, sizeof(user->credentials->password));
    set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, user->role, sizeof(user->role));
    set_binding_param(&param[3], MYSQL_TYPE_LONG, &id, sizeof(id));

    if (mysql_stmt_bind_param(users, param) != 0) {
        print_stmt_error(users, "Unable to bind input parameters");
        goto out;
    }

    if (mysql_stmt_execute(users) != 0) {
        print_stmt_error(users, "An error occurred while registering a user");
        goto out;
    }

    set_binding_param(&param[0], MYSQL_TYPE_LONG, &id, sizeof(id));

    if(mysql_stmt_bind_result(users, param)) {
        print_stmt_error(users, "Unable to bind output parameters");
        goto out;
    }

    while (1) {
        status = mysql_stmt_fetch(users);

        if (status == 1 || status == MYSQL_NO_DATA)
                break;

        snprintf(user->id, sizeof(id), "%d", id);
    }

out:
    while(mysql_stmt_next_result(users) != -1) {}

    mysql_stmt_free_result(users);
    mysql_stmt_reset(users);

    return;
}


void expire_consultation(struct cons *cons)
{
    MYSQL_BIND param[1];

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, cons->cod, sizeof(cons->cod));

    if (mysql_stmt_bind_param(expire, param) != 0) {
        print_stmt_error(expire, "Unable to bind input parameters");
        return;
    }

    if (mysql_stmt_execute(expire) != 0) {
        print_stmt_error(expire, "An error occurred while making a consultation expire");
        return;
    }

    mysql_stmt_free_result(expire);
    mysql_stmt_reset(expire);

    return;
}


struct consultations *get_active_consultations(struct user *user)
{
    MYSQL_BIND param[1];
    int row = 0, status, temp;
    char code[ID_LEN];
    struct consultations *consultations = NULL;

    temp = atoi(user->id);

    set_binding_param(&param[0], MYSQL_TYPE_LONG, &temp, sizeof(temp));

    if (mysql_stmt_bind_param(get_consultation, param) != 0) {
        print_stmt_error(get_consultation, "Unable to bind input parameters");
        goto out;
    }

    if (mysql_stmt_execute(get_consultation) != 0) {
        print_stmt_error(get_consultation, "An error occurred while getting consultation");
        goto out;;
    }

    if (mysql_stmt_store_result(get_consultation)) {
		print_stmt_error(get_consultation, "Unable to store consultations information result set.");
		goto out;
	}

    consultations = malloc(sizeof(*consultations) + sizeof(struct cons) * mysql_stmt_num_rows(get_consultation));
    if(consultations == NULL)
		goto out;
	memset(consultations, 0, sizeof(*consultations) + sizeof(struct cons) * mysql_stmt_num_rows(get_consultation));
	consultations->num_rows = mysql_stmt_num_rows(get_consultation);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, code, sizeof(code));

    if(mysql_stmt_bind_result(get_consultation, param)) {
        print_stmt_error(get_consultation, "Unable to bind output parameters");
        consultations = NULL;
        goto out;
    }

    while (1) {
        status = mysql_stmt_fetch(get_consultation);

        if (status == 1 || status == MYSQL_NO_DATA)
                break;

        strcpy(consultations->consultation[row].cod, code);

        row++;
    }

out:
    while(mysql_stmt_next_result(get_consultation) != -1) {}

    mysql_stmt_free_result(get_consultation);
    mysql_stmt_reset(get_consultation);

    return consultations;
}


void dispose_cons(struct consultations *cons)
{
    free(cons);
}


struct contact *do_register_contact(struct contact *contact)
{
    MYSQL_BIND param[3];
    int id = 0, status, temp;

    temp = atoi(contact->user);

    set_binding_param(&param[0], MYSQL_TYPE_LONG, &id, sizeof(id));
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, contact->client, sizeof(contact->client));
    set_binding_param(&param[2], MYSQL_TYPE_LONG, &temp, sizeof(temp));

    if (mysql_stmt_bind_param(reg_contact, param) != 0) {
        print_stmt_error(reg_contact, "Unable to bind input parameters");
        goto out;
    }

    if (mysql_stmt_execute(reg_contact) != 0) {
        print_stmt_error(reg_contact, "An error occurred while registering a contact");
        goto out;
    }

    set_binding_param(&param[0], MYSQL_TYPE_LONG, &id, sizeof(id));

    if(mysql_stmt_bind_result(reg_contact, param)) {
        print_stmt_error(reg_contact, "Unable to bind output parameters");
        goto out;
    }

    while (1) {
        status = mysql_stmt_fetch(reg_contact);

        if (status == 1 || status == MYSQL_NO_DATA)
                break;

        snprintf(contact->ID, sizeof(id), "%d", id);
    }

out:
    while(mysql_stmt_next_result(reg_contact) != -1) {}

    mysql_stmt_free_result(reg_contact);
    mysql_stmt_reset(reg_contact);

    return contact;
}
