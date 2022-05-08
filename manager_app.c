#include <stdio.h>
#include <string.h>

#include "db.h"
#include "manager.h"
#include "manager_app.h"
#include "io.h"

static bool register_user(void)
{
    struct user user;
    struct credentials cred;
    memset(&user, 0, sizeof(user));
    memset(&cred, 0, sizeof(cred));
    user.credentials = &cred;
    get_user_inf(&user);
    do_register_user(&user);
    print_user_id(&user);
    return false;
}

static bool register_consultation(void)
{
    struct cons cons;
    struct user user;
    struct credentials cred;
    memset(&cons, 0, sizeof(cons));
    memset(&user, 0, sizeof(user));
    memset(&cred, 0, sizeof(cred));
    user.credentials = &cred;
    get_cons_inf(&user, &cons);
    do_register_consultation(&user, &cons);
    return false;
}

static bool expire(void)
{
    struct cons cons;
    memset(&cons, 0, sizeof(cons));
    get_cons_code(&cons);
    expire_consultation(&cons);
    return false;
}

static bool show_consultation(void)
{
    struct user user;
    struct consultations *consultations;
    memset(&user, 0, sizeof(user));
    get_manager_id(&user);
    consultations = get_active_consultations(&user);
    print_consultations(consultations);
    dispose_cons(consultations);
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
	{.action = USER, .control = register_user},
	{.action = CONSULTATION, .control = register_consultation},
    {.action = EXPIRE, .control = expire},
    {.action = SHOW, .control = show_consultation},
	{.action = QUIT, .control = quit}
};

void controller_manager(void)
{
	db_switch_to_manager();

	while(true) {
		int action = get_manager_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}
