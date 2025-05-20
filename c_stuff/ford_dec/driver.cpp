#include "driver.h"

struct device_info {
	char *path;
};

void Driver::constructor(struct device_info)
{
	printf("things, construct arg: %i\n", f_type.a);
}
