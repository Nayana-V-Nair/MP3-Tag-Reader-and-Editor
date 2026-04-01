#ifndef VIEW_H
#define VIEW_H

#include "types.h" // Contains user defined types

typedef struct _ViewInfo
{
    char * mp3_view_fname;
    FILE *fptr_mp3_view;
    int content_size;

} TagView;


/* View function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate View args from argv */
Status read_and_validate_view_arg(char *argv[], TagView *view);

/* Perform the view operation */
Status view_option(TagView *view);

/* Perform view operation */
Status view_operation(TagView *view);

/* Get File pointers for i/p and o/p files */
Status open_files(TagView *view);

/* Fetching Tags */
void fetch_tag(TagView *view);

/* Get content size */
void content_size(TagView *view);

/* Get content */
void content(TagView *view);

/* To close the file */
void file_close_view(TagView *view);

/* Perform help operation */
void help_option();

 #endif