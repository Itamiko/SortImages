#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

enum { FALSE = 0, TRUE = 1 };
enum { LIST_MODE = TRUE, CREATE_MODE = FALSE } mode;

int x, y;
int width, height;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

int main(int argc, char *argv[])
{
	size_t optind;
	for (optind = 1; optind < argc && argv[optind][0] == '-'; optind++) {
		switch (argv[optind][1])
		{
		case 'l':
			mode = LIST_MODE;
			break;
		case 'c':
			mode = CREATE_MODE;
			break;
		default:
			printf("Usage: %s [-lc] [dir...]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	DIR *path;
	struct dirent *dp;

	path = opendir(argv[2]);
	if (path == NULL) {
		printf("Directory does not exist");
		exit(EXIT_FAILURE);
	}

	int i = 0;
	char *new_str;
	char header[8];
	while((dp = readdir(path)) != NULL) {
		if(strcmp(".", dp->d_name) != 0 && strcmp("..", dp->d_name) != 0) {
			if (mode == CREATE_MODE) {
				if((new_str = malloc(strlen(argv[2])+strlen(dp->d_name)+1)) != NULL){

					new_str[0] = '\0';
					strcat(new_str, argv[2]);
					strcat(new_str, dp->d_name);

					FILE *fp = fopen(new_str, "rb");
					if (!fp) {
						printf("[read_png_file] File %s could not be opened for reading", new_str);
					}
					fread(header, 1, 8, fp);
					//if (png_sig_cmp(header, 0, 8)) {
					//	printf("[read_png_file] File %s is not recognized as a PNG file", dp->d_name);
					//}
					png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
					info_ptr = png_create_info_struct(png_ptr);
					png_init_io(png_ptr, fp);
					png_set_sig_bytes(png_ptr, 8);
					png_read_info(png_ptr, info_ptr);
					width = png_get_image_width(png_ptr, info_ptr);
					height = png_get_image_height(png_ptr, info_ptr);

					if (setjmp(png_jmpbuf(png_ptr)))
						printf("[read_png_file] Error during read_image");

					row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
					for (y=0; y<height; y++)
						row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

					png_read_image(png_ptr, row_pointers);

					printf("[%s] %ix%i", dp->d_name, width, height);
					fclose(fp);
				} else {
					printf("Memory allocation failed");
					exit(EXIT_FAILURE);
				}
			} else if (mode == LIST_MODE) {
				i++;
				printf("%i: %s\n", i, dp->d_name);
			}
		}
	}
	closedir(path);
	return EXIT_SUCCESS;
}