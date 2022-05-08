#pragma once
#include "db.h"

extern bool ask_for_relogin(void);
extern void view_login(struct credentials *cred);
extern bool login(void);
