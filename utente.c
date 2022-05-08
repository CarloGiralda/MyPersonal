#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "utente.h"
#include "io.h"
#include "validation.h"

int get_user_action(void)
{
	char options[10] = {'1','2','3','4','5','6','7','8','9','0'};
	char op;

	clear_screen();
	puts("**************************");
	puts("*    USER'S DASHBOARD    *");
	puts("**************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Show agenda");
	puts("2) Show client");
	puts("3) Show my clients");
	puts("4) Insert note");
	puts("5) Modify note");
	puts("6) Delete note");
	puts("7) Offer an opportunity");
	puts("8) Register an appointment");
    puts("9) Register a contact");
	puts("0) Quit");

	op = multi_choice("Select an option", options, 10);
    if(op == '0')
        return 9;
	return op - '1';
}

void get_user_id(struct user *user)
{
	clear_screen();
	puts("** Get your personal information **\n");

	get_input("Insert your ID: ", ID_LEN, user->id, false);
}

void print_agenda(struct appointments *app)
{
	clear_screen();
	puts("** Agenda **\n");

	for(int i = 0; i < app->num_rows; i++) {
		printf("\n%s %s %s\n", app->appointment[i].day, app->appointment[i].time, app->appointment[i].place);
	}
}

void get_client_id(struct client *client)
{
	clear_screen();
	puts("** Get information about a specific client **\n");

	get_input("Insert client's Fiscal Code: ", ID_LEN, client->code, false);
}

void print_client(struct fullclient *fc)
{
	clear_screen();
    puts("** Client's informations **\n");

    printf("\nFiscal Code: %s", fc->client->code);
    printf("\nName: %s", fc->client->name);
    printf("\nSurname: %s", fc->client->surname);
    printf("\nBirthday: %s", fc->client->birthday);
    printf("\nDay of registration: %s", fc->client->registration);
    printf("\nFax: %s", fc->client->fax);
    printf("\nEmail: %s", fc->client->email);
    printf("\nAddress: %s", fc->client->address);
    printf("\nPhone number: %s", fc->client->phone);
    printf("\nID of the user who added it: %s", fc->client->comm);
    printf("\nID of the user who is related: %s", fc->client->funz);
    if(atoi(fc->client->IVA) != 0) printf("\nIVA: %s\n", fc->client->IVA);
    else printf("\nThis client has no IVA\n");

    puts("\n** Client's appoinments and contacts **\n");

    int row = 0;

    if(fc->note != NULL){
        while(fc->note[row] != NULL) row++;

        for(size_t i = 0; i < row; i++) {
            printf("\n%s\n", fc->note[i]->number);
        }
    }

    puts("\n** Client's bought consultations **\n");

    row = 0;

    if(fc->bought != NULL){
        while(fc->bought[row] != NULL) row++;

        for(size_t i = 0; i < row; i++) {
            printf("\n%s\n", fc->bought[i]->cod);
        }
    }
}

void show_clients(struct client **clients)
{
    clear_screen();
    puts("** Your Clients **\n");

    int row = 0;

    while(clients[row] != NULL) row++;

    for(int i = 0; i < row; i++) {
		printf("\n%s\n", clients[i]->code);
	}
}

void ins_note(struct note *note, struct cons *cons)
{
    clear_screen();
	puts("** Get note's datas **\n");

    get_input("Insert note's ID: ", ID_LEN, note->number, false);
    get_input("Insert ID of the user who is writing it: ", ID_LEN, note->func, false);
    get_input("Insert text: ", TEXT_LEN, note->text, false);
    get_input("Insert appointment or contact's ID: ", ID_LEN, note->input, false); //serve per distinguere se ci si riferisce ad un contatto o ad un appuntamento
    get_input("Insert consultation's ID if someone has received affermative answer: ", ID_LEN, cons->cod, false);
}

void mod_note(struct note *note)
{
    clear_screen();
	puts("** Get note's datas **\n");

    get_input("Insert Note's ID: ", ID_LEN, note->number, false);
    get_input("Insert text: ", TEXT_LEN, note->text, false);
}

void del_note(struct note *note)
{
    clear_screen();
	puts("** Get note's ID **\n");

    get_input("Insert Note's ID: ", ID_LEN, note->number, false);
}

void get_opportunity(struct cons *cons)
{
    get_input("Insert opportunity's ID: ", ID_LEN, cons->cod, false);
}

void get_appointment_information(struct appointment *app)
{
    clear_screen();
	puts("** Get appointment information **\n");

    while(true) {
		if(validate_date(get_input("Insert day [YYYY-MM-DD]: ", DATE_LEN, app->day, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}

    while(true) {
		if(validate_time(get_input("Insert time [XX:XX]: ", TIME_LEN, app->time, false)))
			break;
		fprintf(stderr, "Invalid time!\n");
	}

	get_input("Insert location: ", LOCATION_LEN, app->place, false);

    while(true) {
        if(atoi(get_input("Insert user's ID: ", ID_LEN, app->func, false)) != 0)
            break;
        fprintf(stderr, "Invalid ID!\n");
    }

    get_input("Insert client's ID: ", ID_LEN, app->client, false);
}

void print_appointment(int cod)
{
    clear_screen();
    puts("** Your Appointment ID **\n");

    printf("\n%d\n", cod);
}

void get_contact_inf(struct contact *contact)
{
    clear_screen();
    puts("** Get contact information **\n");

    while(true) {
        if(atoi(get_input("Insert user's ID: ", ID_LEN, contact->user, false)) != 0)
            break;
        fprintf(stderr, "Invalid ID!\n");
    }

    get_input("Insert client's ID: ", ID_LEN, contact->client, false);
}

void print_contact_id(struct contact *contact)
{
    clear_screen();
    puts("** Your Contact ID **\n");

    printf("\n%d\n", atoi(contact->ID));
}
