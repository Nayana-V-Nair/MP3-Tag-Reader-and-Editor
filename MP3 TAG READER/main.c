/*
NAME : NAYANA V NAIR
DESCRIPTION : To implement MP3 Tag Reader & Editor
DATE : 12/11/2025
*/

#include <stdio.h>
#include <string.h>
#include "view.h"
#include "types.h"
#include "edit.h"

int main(int argc, char *argv[])  
{
    //Validating arguments
    if( argc < 2 || ((strcmp(argv[1], "-v") == 0) && argc < 3) || ((strcmp(argv[1], "-e") == 0) && argc < 5) )
    {
        //error messages
        printf("ERROR: ./a.out: INVALID ARGUMENTS\n");
        printf("USAGE:\n");

        printf("\t To view please pass like: ./a.out -v mp3filename\n");

        printf("\t To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");

        printf("\t To get help pass like: ./a.out --help\n");

        return e_failure;
    }
    
   if(check_operation_type(argv) == e_view)    //checks if the operation selected is view option
   {
    printf("<---------You have SELECTED VIEW OPTION--------->\n");
    TagView view;      //declaring a structure variable for view option

    if(read_and_validate_view_arg(argv, &view) == e_success)   //function call to read and validate the view option arguments
    {
        printf("Read and Validate is successful\n");
        if(view_option(&view) == e_success)         //function call to perform view operations
        {
            printf("<------VIEWING SUCCESSFUL------>\n");
        }
        else
        {
            printf("VIEWING UNSUCCESSFUL\n");
            return e_failure;
        } 
    }
    else
    {
        printf("Read and Validate is unsuccessful\n");
        return e_failure;
    }
   }

   else if(check_operation_type(argv) == e_edit)     //checks if the operation selected is edit option
   {
    printf("<---------You have SELECTED EDIT OPTION--------->\n");
    TagEdit edit;          //declaring a structure variable for editing operation

    if(read_and_validate_edit_args(argv, &edit) == e_success)  //function call to read and validate the edit arguments
    {
        printf("Read and Validate is successful\n");
        if(edit_option(&edit, argv) == e_success)         //function call to perform edit operation
        {
            printf("<------EDITING SUCCESSFUL------>\n");
        }
        else
        {
            printf("EDITING UNSUCCESSFUL\n");
            return e_failure;
        }
    }
    else
    {
        printf("Read and Validate is unsuccessful\n");
        return e_failure;
    }
   
   }


   else if(check_operation_type(argv) == e_help)     //checks if the operation selected is help option
   {
    printf("<---------You have SELECTED HELP OPTION--------->\n");

        help_option();         //function call to perform help operation
   }

   else
   {
    printf("<------------IT IS UNSUPPORTED------------>\n");
    return e_failure;
   }

   return e_success;

}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-v") == 0)    //checks the operation type (view)
    {
        return e_view;
    }
    else if(strcmp(argv[1], "-e") == 0)  //checks the operation type (edit)
    {
        return e_edit;
    }
    else if(strcmp(argv[1], "--help") == 0)  //checks the operation type (help)
    {
        return e_help;
    }
    else
    {
        return e_unsupported;
    }
}
