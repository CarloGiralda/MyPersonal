#include <stdio.h>

#include "login.h"
#include "db.h"
#include "io.h"
#include "validation.h"


#define check_env_failing(varname)                                          \
if(getenv((varname)) == NULL) {                                             \
        fprintf(stderr, "[FATAL] env variable %s not set\n", (varname));    \
        ret = false;                                                        \
}
static bool validate_dotenv(void)
{
    setenv("HOST", "localhost", 1);
    setenv("DB", "azienda", 1);
    setenv("LOGIN_USER", "login", 1);
    setenv("LOGIN_PASS", "login", 1);
    setenv("FUNC_USER", "funzionario", 1);
    setenv("FUNC_PASS", "funzionario", 1);
    setenv("MANAGER_USER", "manager", 1);
    setenv("MANAGER_PASS", "manager", 1);
    setenv("COMMERCIALE_USER", "commerciale", 1);
    setenv("COMMERCIALE_PASS", "commerciale", 1);

	bool ret = true;

	check_env_failing("HOST");
	check_env_failing("DB");
	check_env_failing("LOGIN_USER");
	check_env_failing("LOGIN_PASS");
	check_env_failing("FUNC_USER");
	check_env_failing("FUNC_PASS");
	check_env_failing("MANAGER_USER");
	check_env_failing("MANAGER_PASS");
    check_env_failing("COMMERCIALE_USER");
	check_env_failing("COMMERCIALE_PASS");

	return ret;
}
#undef set_env_failing

int main()
{
	if(!validate_dotenv())
		return 1;
	if(!init_validation())
		return 1;
	if(!init_db())
        return 1;

	if(initialize_io()) {
		do {
			if(!login())
				fprintf(stderr, "Login unsuccessful\n");
			db_switch_to_login();
		} while(ask_for_relogin());
	}
	fini_db();
	fini_validation();

	puts("Bye!");
	return 0;
}
