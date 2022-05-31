#include <stdio.h>
#include <string.h>

#include "db.h"
#include "manager.h"
#include "io.h"
#include "validation.h"

int get_manager_action(void)
{
	char options[5] = {'1','2','3','4','5'};
	char op;

	clear_screen();
	puts("**************************");
	puts("*    USER'S DASHBOARD    *");
	puts("**************************\n");
	puts("*** What should I do for you? ***\n");
	puts("1) Add a user");
	puts("2) Add a consultation");
    puts("3) Make a consultation expire");
    puts("4) Show active consultations");
	puts("5) Quit");

	op = multi_choice("Select an option", options, 5);
	return op - '1';
}

void get_user_inf(struct user *user)
{
	clear_screen();
	puts("** Get information about user **\n");

	get_input("Insert username: ", USERNAME_LEN, user->credentials->username, false);
	get_input("Insert password: ", PASSWORD_LEN, user->credentials->password, false);
	get_input("Insert role: ", NAME_LEN, user->role, false);
}

void print_user_id(struct user *user)
{
	clear_screen();
	puts("** User's ID **\n");

    if(strcmp(user->id, "") == 0) {
        printf("\nInvalid role. Please enter only [utente], [manager] or [commerciale]\n");
        return;
    }
	printf("\n%s\n", user->id);
}

void get_cons_inf(struct user *user, struct cons *cons)
{
	clear_screen();
	puts("** Get information about consultation **\n");

    get_input("Insert your username: ", USERNAME_LEN, user->credentials->username, false);
	get_input("Insert Code: ", ID_LEN, cons->cod, false);
    while(true) {
		if(validate_date(get_input("Insert activation date [YYYY-MM-DD]: ", DATE_LEN, cons->day, false)))
			break;
		fprintf(stderr, "Invalid date!\n");
	}
}

void get_cons_code(struct cons *cons)
{
    clear_screen();

	puts("** Get information about expiring consultation **\n");

	get_input("Insert Code: ", ID_LEN, cons->cod, false);
}

void get_manager_id(struct user *user)
{
    clear_screen();
	puts("** Get information about manager **\n");

	get_input("Insert your ID: ", ID_LEN, user->id, false);
}

void print_consultations(struct consultations *consultations)
{
    clear_screen();
    puts("** Showing your consultations still active **\n");

    printf("Number of consultations still active: %d\n", consultations->num_rows);

    for(int i = 0; i < consultations->num_rows; i++) {
        printf("\n%s\n", consultations->consultation[i].cod);
    }
}
