#include <iostream>

enum board_state {
	EMPTY, XS, OS
};

void get_move(int *x, int *y)
{
	std::cout << "enter a column (1, 2, 3): ";
	std::cin >> *x;

	std::cout << "enter a row (A, B, C): "; 
	std::cin >> *y;
}

char get_state_char(enum board_state s)
{
	switch (s)
	{
	case EMPTY: return ' ';
	case XS: return 'X';
	case OS: return 'O';
	default: return '?';
	}
}

void print_board(board_state board[])
{
	std::cout << " 123\n";
	for (int i = 0; i < 3; i++) {
		std::cout << static_cast<char>(i + 'A');
		for (int j = 0; j < 3; j++) {
			std::cout << get_state_char(board[j + (i * 3)]);
		}
		std::cout << "\n";
	}
}

void play_round(board_state board[])
{
	int move_x = -1, move_y = -1;
	get_move(&move_x, &move_y);

	std::cout << move_y;
	board[(move_x - 1) + ((move_y) * 3)] = XS;

	std::cout << "AI is making their move...\n";
}

int main()
{
	std::cout << "creating board\n";

	enum board_state this_board[9] = {EMPTY};
	print_board(this_board);

	std::cout << "finished board\n";

	play_round(this_board);
	print_board(this_board);

	return 0;
}
