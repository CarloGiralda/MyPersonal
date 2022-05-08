#include <stdio.h>
#include <string.h>

#include "db.h"
#include "commerciale.h"
#include "commerciale_app.h"
#include "io.h"

static bool register_client(void)
{
	struct client client;
	memset(&client, 0, sizeof(client));
	get_client_inf(&client);
	do_register_client(&client);
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
	{.action = CLIENT, .control = register_client},
	{.action = QUIT, .control = quit}
};

void controller_commerciale(void)
{
	db_switch_to_comm();

	while(true) {
		int action = get_comm_action();
		if(action >= END_OF_ACTIONS) {
			fprintf(stderr, "Error: unknown action\n");
			continue;
		}
		if (controls[action].control())
			break;

		press_anykey();
	}
}
