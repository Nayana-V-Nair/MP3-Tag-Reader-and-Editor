#ifndef EDIT_H
#define EDIT_H

#include "types.h" // Contains user defined types

typedef struct _EditInfo
{
    char * mp3_edit_fname;

    FILE *fptr_mp3_edit;

    FILE *fptr_original;

    //int e_content_size;

    int content_size_e;

} TagEdit;


/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate edit args from argv */
Status read_and_validate_edit_args(char *argv[], TagEdit *edit);

void copy_mp3_header(FILE *fptr_mp3_edit, FILE *fptr_original);

/* Perform the edit operation */
Status edit_option(TagEdit *edit, char *argv[]);

/* Get File pointers for i/p and o/p files */
Status open_files_edit(TagEdit *edit);

/* Perform edit operation */
void edit_operations(char *ptr, TagEdit *edit, char *argv[]);

/* Perform edit operation */
//void edit_content_size(TagEdit *edit);

/* Get content size */
void content_size_edit(TagEdit *edit);


#endif