#include <stdio.h>

#include "map.h"

int main()
{
	map_t glyphs;
	create_map(&glyphs);

	// usual use case
	char key_a = 'a';
	char key_b = 'b';
	char key_c = 'c';

	struct val {
		int x, y;
	};

	struct val val_a = { 36, 36 };
	struct val val_b = { 24, 24 };
	struct val val_c = { 12, 12 };

	map_add(&glyphs, &key_a, &val_a);
	map_add(&glyphs, &key_b, &val_b);
	map_add(&glyphs, &key_c, &val_c);

	struct val test;
	map_get(&glyphs, &key_b, &test);
	printf("x: %u, y: %u\n", test.x, test.y);

	destroy_map(&glyphs);

	// loop use case
	
	for (int i = 0; i < 10; i++) {
		char key = i + '0';
		struct val value = {i, i};

		map_add(&glyphs, &key, &value);
	}

	print_map(&glyphs);

	return 0;
}
