#include <stdio.h>

#include "db.h"
#include "commerciale.h"
#include "io.h"
#include "validation.h"

int get_comm_action(void)
{
	char options[2] = {'1','2'};
	char op;

	clear_screen();
	puts("**************************");
	puts("*    COMMERCIAL'S DASHBOARD    *");
	puts("**************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Register a new client");
	puts("2) Quit");

	op = multi_choice("Select an option", options, 2);
	return op - '1';
}

void get_client_inf(struct client *client)
{
	clear_screen();
	puts("** Get client's information **\n");

	get_input("Insert FiscalCode: ", ID_LEN, client->code, false);
	get_input("Insert first name: ", NAME_LEN, client->name, false);
	get_input("Insert surname: ", SURNAME_LEN, client->surname, false);
    while(true) {
		if(validate_date(get_input("Insert birthday [YYYY-MM-DD]: ", DATE_LEN, client->birthday, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}
    while(true) {
		if(validate_date(get_input("Insert registration date [YYYY-MM-DD]: ", DATE_LEN, client->registration, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}
	while(true) {
        if(atoi(get_input("Insert fax: ", NUMBER_LEN, client->fax, false)) != 0)
            break;
        fprintf(stderr, "Invalid fax!\n");
    }
	get_input("Insert email: ", NUMBER_LEN, client->email, false);
	get_input("Insert address: ", NUMBER_LEN, client->address, false);
    while(true) {
        if(atoi(get_input("Insert phone: ", NUMBER_LEN, client->phone, false)) != 0)
            break;
        fprintf(stderr, "Invalid phone!\n");
    }
	get_input("Insert user's ID who is adding client: ", ID_LEN, client->comm, false);
	get_input("Insert user's ID who is related to client: ", ID_LEN, client->funz, false);
    get_input("Insert user's IVA if he/she has one: ", NAME_LEN, client->IVA, false);
}
