#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include<stdio.h>
#include <string.h>
#include <stdlib.h>

char *dir_parser(char *full_path) ;
char *db_class_no_parser(char *db_line);
char *class_no_parser(char *full_path);
char *db_side_parser(char *db_line);
char *side_parser(char *full_path);
char *db_name_parser (char *db_line) ;
unsigned char *db_bit_parser (char *db_line);
unsigned char *db_bit_mask_parser (char *db_line);
char *name_parser(char *full_path) ;
char *build_result_path (char *full_path, char *file_name) ;

#endif // PARSER_H_INCLUDED
