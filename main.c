#include "ui.h"


int main() {
	init();
	print_gold(100);
	print_soldiers(15);
	print_zombies(5, 13);
	print_health(100);


	while(1) {
		int ch = get_input();
		switch(ch) {
			case 'q':
				game_end(1);
				break;
		}
	}
}
