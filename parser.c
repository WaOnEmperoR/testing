#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "parser.h"

char *dir_parser(char *full_path)
{
	int i;

	int limit;

	for (i = strlen(full_path); i >= 0; i--) {
		if (full_path[i] == '/') {
			limit = i;
			break;
		}
	}

	char *dir_path = malloc((limit + 1) * sizeof(char));
	for (i = 0; i < limit + 1; i++)
		dir_path[i] = '\0';

	strncpy(dir_path, full_path, limit);

	//printf("dir_path = %s\n", dir_path);

	return dir_path;
}

char *db_class_no_parser(char *db_line)
{
	int i;

	int limit = 0;

	for (i = 0; i < strlen(db_line); i++) {
		if (db_line[i] == '_') {
			limit = i;
			break;
		}
	}
	//printf("db_limit = %d\n", limit);

	char *db_class_no = malloc(4 * sizeof(char));
	for (i = 0; i < 4; i++)
		db_class_no[i] = '\0';
	strncpy(db_class_no, db_line, limit);

//	printf("db_class_no %s\n", db_class_no);
	return db_class_no;
}

char *class_no_parser(char *full_path)
{
	int i;

	int limit_1;
	int limit_2;

	char *class_no = malloc(4 * sizeof(char));

	for (i = 0; i < 4; i++)
		class_no[i] = '\0';

	for (i = 0; i < strlen(full_path); i++) {
		if (full_path[i] == '/') {
			limit_1 = i;
			break;
		}
	}
	//printf("limit_1 %d\n", limit_1);

	for (i = limit_1 + 1; i < strlen(full_path); i++) {
		if (full_path[i] == '/') {
			limit_2 = i;
			break;
		}
	}
	//printf("limit_2 %d\n", limit_2);

	strncpy(class_no, full_path + limit_1 + 1, limit_2 - limit_1 - 1);
//	printf("CLASS_NO %s\n", class_no);
	return class_no;
}

char *db_side_parser(char *db_line)
{
	int i;

	int limit_1;
	int limit_2;

	char *db_side = malloc(8 * sizeof(char));

	for (i = 0; i < 8; i++)
		db_side[i] = '\0';

	for (i = 0; i < strlen(db_line); i++) {
		if (db_line[i] == '_') {
			limit_1 = i;
			break;
		}
	}
//	printf("db_limit_1 = %d\n", limit_1);

	for (i = limit_1 + 1; i < strlen(db_line); i++) {
		if (db_line[i] == '_') {
			limit_2 = i;
			break;
		}
	}
//	printf("db_limit_2 = %d\n", limit_2);

	strncpy(db_side, db_line + limit_1 + 1, limit_2 - limit_1 - 1);
//	printf("db_side %s\n", db_side);
	return db_side;
}

char *side_parser(char *full_path)
{
	int i;

	int limit_1;
	int limit_2;

	char *side = malloc(8 * sizeof(char));

	for (i = 0; i < 8; i++)
		side[i] = '\0';

	int slash = 0;
	for (i = 0; i < strlen(full_path); i++) {
		if (full_path[i] == '/') {
			slash++;
			if (slash == 2) {
				limit_1 = i;
				break;
			}
		}
	}
//	printf("limit_1 %d\n", limit_1);

	for (i = limit_1 + 1; i < strlen(full_path); i++) {
		if (full_path[i] == '/') {
			limit_2 = i;
			break;
		}
	}
//	printf("limit_2 %d\n", limit_2);

	strncpy(side, full_path + limit_1 + 1, limit_2 - limit_1 - 1);
	//printf("SIDE %s\n", side);

	return side;
}

char *db_name_parser (char *db_line)
{
	int i;

	int limit_1;
	int limit_2;

	int uscore = 0;
	for (i = 0; i < strlen(db_line); i++) {
		if (db_line[i] == '_') {
			uscore++;
			if (uscore == 2) {
				limit_1 = i;
				break;
			}
		}
	}
//	printf("db_limit_1 %d\n", limit_1);

	for (i = limit_1 + 1; i < strlen(db_line); i++) {
		if (db_line[i] == '_') {
			limit_2 = i;
			break;
		}
	}
//	printf("db_limit_2 %d\n", limit_2);

	char *db_name = malloc((limit_2 - limit_1) * sizeof(char));

//	printf("len db_name = %d\n", limit_2 - limit_1);
	for (i = 0; i < (limit_2 - limit_1); i++)
		db_name[i] = '\0';
//	printf("db_name = %s\n", db_name);
	strncpy(db_name, db_line + limit_1 + 1, limit_2 - limit_1 - 1);
	//printf("db_name %s\n", db_name);

	return db_name;
}

unsigned char *db_bit_parser (char *db_line)
{
	int i;

	int limit;

	int uscore = 0;
	for (i = 0; i < strlen(db_line); i++) {
		if (db_line[i] == '_') {
			uscore++;
			if (uscore == 3) {
				limit = i;
				break;
			}
		}
	}
//	printf("limit %d\n", limit);

	unsigned char *db_bit = (unsigned char*) malloc((RADIUS * THETA * 2) * sizeof(unsigned char));
	char *db_bit_str = (char*)malloc((RADIUS * THETA * 2) * sizeof(char));
	for (i = 0; i < (RADIUS * THETA * 2); i++) {
		db_bit_str[i] = '\0';
	}

	for (i = 0; i < (RADIUS * THETA * 2); i++) {
		db_bit[i] = 0;
	}

	strncpy(db_bit_str, db_line + limit + 1, (RADIUS * THETA * 2));
//	printf("db_bit_str = %s\n", db_bit_str);
	for (i = 0; i < (RADIUS * THETA * 2); i++) {
//		char *a = malloc(2 * sizeof(char));
//		a[0] = db_bit_str[i];
//		a[1] = '\0';
//		sprintf(a, "%d", db_bit[i]);
		db_bit[i] = db_bit_str[i] - '0';
//		printf("db_bit[%d] = %d\n", i, db_bit[i]);
	}

	return db_bit;
}

unsigned char *db_bit_mask_parser (char *db_line)
{
	int i;

	int limit;

	int uscore = 0;
	for (i = 0; i < strlen(db_line); i++) {
		if (db_line[i] == '_') {
			uscore++;
			if (uscore == 4) {
				limit = i;
				break;
			}
		}
	}

	unsigned char *db_bit_mask = (unsigned char*)malloc((RADIUS * THETA * 2) * sizeof(char));
	char *db_bit_mask_str = (char*)malloc((RADIUS * THETA * 2) * sizeof(char));

	for (i = 0; i < (RADIUS * THETA * 2); i++)
		db_bit_mask_str[i] = '\0';

	for (i = 0; i < (RADIUS * THETA * 2); i++) {
		db_bit_mask[i] = 0;
	}

	strncpy(db_bit_mask_str, db_line + limit + 1, (RADIUS * THETA * 2));

//	printf("db_bit_mask %s\n", db_bit_mask_str);
	for (i = 0; i < (RADIUS * THETA * 2); i++) {
//		char *a = malloc(2 * sizeof(char));
//		a[0] = db_bit_mask_str[i];
//		a[1] = '\0';
		db_bit_mask[i] = db_bit_mask_str[i] - '0';
//		printf("%d", db_bit_mask[i]);
//		sprintf(a, "%d", db_bit_mask[i]);
	}
//	printf("\n");
	return db_bit_mask;
}

char *name_parser(char *full_path)
{
	int i;

	int limit_slash;
	int limit_dot;

//	printf("full_path %s\n", full_path);
	for (i = strlen(full_path); i >= 0; i--) {
		if (full_path[i] == '/') {
			limit_slash = i;
			break;
		}
	}
//	printf("limit_slash %d\n", limit_slash);

	for (i = strlen(full_path); i >= 0; i--) {
		if (full_path[i] == '.') {
			limit_dot = i;
			break;
		}
	}
//	printf("limit_dot %d\n", limit_dot);
	/* Now, use strncpy() to copy only the last 13 characters. */
//     strncpy(dest_str2, source_str + (strlen(source_str) - 13), 13);
	//printf("%d\n", strlen(full_path) - (limit_dot - limit_slash));
	char *name = malloc((limit_dot - limit_slash) * sizeof(char));
	for (i = 0; i < (limit_dot - limit_slash); i++)
		name[i] = '\0';
	strncpy(name, full_path + limit_slash + 1, limit_dot - limit_slash - 1);

//	printf("name = %s\n", name);

	return name;
}

char *build_result_path (char *full_path, char *file_name)
{
	char *result_path = malloc((strlen(full_path) + strlen(file_name) + 2) * sizeof(char));

	sprintf(result_path, "%s/%s\0",full_path, file_name);
	//printf("result_path %s\n", result_path);
	return result_path;
}


