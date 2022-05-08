#pragma once
#include "db.h"

enum actions {
    CLIENT,
    QUIT,
    END_OF_ACTIONS
};

int get_comm_action(void);
void get_client_inf(struct client *client);
