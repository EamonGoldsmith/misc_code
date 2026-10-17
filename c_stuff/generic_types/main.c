#include <stdio.h>
#include <stdint.h>

void set_int(const char *name, int i)
{
	printf("setting int %i, for %s\n", i, name);
}

void set_char(const char *name, char c)
{
	printf("setting char %c, for %s\n", c, name);
}

void set_float(const char *name, float f)
{
	printf("setting float %f, for %s\n", f, name);
}

#define set_property(name, value) \
	_Generic ((value), \
		char: set_char, \
		int: set_int, \
		float: set_float \
	)(name, value)

int main()
{
	// test
	set_int("test", 3);

	// generic:
	set_property("fthign", 3.14f);
	set_property("cthign", (char)'a');
	set_property("ithign", 67);
	set_property("djwioa", (char)'z');

	return 0;
}
