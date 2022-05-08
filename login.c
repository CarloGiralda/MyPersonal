#include <stdbool.h>
#include <stdio.h>

#include "login.h"
#include "io.h"
#include "db.h"
#include "utente_app.h"
#include "manager_app.h"
#include "commerciale_app.h"

bool login(void)
{
	struct credentials cred;
	view_login(&cred);
	role_t role = attempt_login(&cred);

	switch(role) {
		case UTENTE:
			controller_utente();
			break;
		case MANAGER:
			controller_manager();
			break;
		case COMMERCIALE:
			controller_commerciale();
			break;
		default:
			return false;
	}

	return true;
}

void view_login(struct credentials *cred)
{
	clear_screen();
	puts("**************");
	puts("* CRM SYSTEM *");
	puts("**************\n");
	get_input("Username: ", USERNAME_LEN, cred->username, false);
	get_input("Password: ", PASSWORD_LEN, cred->password, true);
}

bool ask_for_relogin(void)
{
	return yes_or_no("Do you want to log in as a different user?", 'y', 'n', false, true);
}
