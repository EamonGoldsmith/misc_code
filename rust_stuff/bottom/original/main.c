#include <ncurses.h>

int main()
{
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(1);

	if (has_colors() == FALSE) {
		endwin();
		printf("your terminal doesn't support colours\n");
		return 1;
	}

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);

	attron(COLOR_PAIR(1));

	int x, y;
	getmaxyx(stdscr, y, x);
	y = y * 0.5;
	x = (x * 0.5) - 6;
	
	mvwprintw(stdscr, y, x, "hello, world!");
	mvwprintw(stdscr, y, x, "hello, world!");
	refresh();
	
	attroff(COLOR_PAIR(1));

	getch();

	endwin();

	return 0;
}
