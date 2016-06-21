#include <shell.h>
#include <tests.h>

static int test = 1;

int main() {

	if (test) {
		run_tests();
	}

	init_shell();

	while (1) {
		update_shell();
	}

	return 0;
}