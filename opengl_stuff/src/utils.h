
#ifndef UTILS_H_
#define UTILS_h_

static inline char* load_file(const char *path, size_t *len)
{
    FILE *fp = fopen(path, "r");

    if (!fp) {
		*len = 0;
		fprintf(stderr, "Failed to open file: %s\n", path);
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	rewind(fp);

	char *buffer = malloc(sizeof(char) * (file_size + 1));
	if (!buffer) {
		*len = 0;
		fprintf(stderr, "Failed to allocate file buffer\n");
		return NULL;
	}

	size_t bytes_read = fread(*buffer, sizeof(char), file_size, fp);
	(*buffer)[file_size] = '\0';

	if (file_size != bytes_read) {
		free(*buffer);
		*buffer = NULL;
	}

	fclose(fp);
	*len = bytes_read;
	printf("Loaded file %s, %lu bytes read\n", path, bytes_read);

	return buffer;
}

#endif // UTILS_H_
