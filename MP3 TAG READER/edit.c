#include <stdio.h>
#include "edit.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>

//  Validating arguments
Status read_and_validate_edit_args(char *argv[], TagEdit *edit)
{
    if(strstr(argv[3], ".mp3") != NULL)
    {
        printf("ERROR :- The New Content is not present...!\n");
        return e_failure;
    }
    
    if (strstr(argv[4], ".mp3") != NULL)   //checks if .mp3 is present or not
    {
        printf(".mp3 is present\n");
        edit->mp3_edit_fname = argv[4];  //collecting the argument in another variable
    }
    else
    {
        printf("ERROR : The .mp3 extension is not proper!\n");
        return e_failure;
    }

    return e_success;
}

//  Opening both files 
Status open_files_edit(TagEdit *edit)
{
    edit->fptr_mp3_edit = fopen("duplicate.mp3", "w"); //opens duplicate.mp3 in write mode
    if (edit->fptr_mp3_edit == NULL)         //checks if file is present or not
    {
        printf("ERROR: Could not create duplicate.mp3\n");
        return e_failure;
    }

    edit->fptr_original = fopen(edit->mp3_edit_fname, "r"); //opens the original.mp3 in read mode
    if (edit->fptr_original == NULL)            //checks if file is present or not
    {
        printf("ERROR: Could not open original.mp3\n");
        fclose(edit->fptr_mp3_edit);
        return e_failure;
    }

    printf("All files opened successfully!\n");
    return e_success;
}

//  Copying first 10-byte header
void copy_mp3_header(FILE *fptr_mp3_edit, FILE *fptr_original)
{
    rewind(fptr_original);   //makes the file pointer to point at the beginning of the file
    
    unsigned char arr[10];   //declaring an array to collect 10 header bytes
    
    fread(arr, 10, 1, fptr_original); //reads the 10 header bytes from original.mp3
    
    fwrite(arr, 10, 1, fptr_mp3_edit); //writing the 10 header bytes to duplicate.mp3
}

// Function to write size in big-endian order
void write_big_endian(FILE *fp, unsigned int num)
{
    unsigned char bytes[4];    //declaring an array to collect the size
    
    /* converting the integer data to big endian */
    bytes[0] = (num >> 24) & 0xFF;
    bytes[1] = (num >> 16) & 0xFF;
    bytes[2] = (num >> 8) & 0xFF;
    bytes[3] = num & 0xFF;
    
    fwrite(bytes, 1, 4, fp);
}

//  Reads the size
unsigned int read_big_endian(FILE *fp)
{
    unsigned char size[4];  //declaring an array to collect the size
    
    fread(size, 1, 4, fp);  //reads 4 bytes from the file
    
    return (size[0] << 24) | (size[1] << 16) | (size[2] << 8) | size[3]; //converting the big endian data to integer
}

//   Edit option
Status edit_option(TagEdit *edit, char *argv[])
{
    if (open_files_edit(edit) != e_success)  
    {
        return e_failure;
    }

    copy_mp3_header(edit->fptr_mp3_edit, edit->fptr_original); //function call to copy header bytes

    if (strcmp(argv[2], "-t") == 0)   //checks if the argument passed is '-t'
        edit_operations("TIT2", edit, argv);
    
    else if (strcmp(argv[2], "-a") == 0)    //checks if the argument passed is '-a'
        edit_operations("TPE1", edit, argv);
    
    else if (strcmp(argv[2], "-A") == 0)    //checks if the argument passed is '-A'
        edit_operations("TALB", edit, argv);
    
    else if (strcmp(argv[2], "-m") == 0)    //checks if the argument passed is '-m'
        edit_operations("TCON", edit, argv);
   
    else if (strcmp(argv[2], "-y") == 0)   //checks if the argument passed is '-y'
        edit_operations("TYER", edit, argv);
        
    else if (strcmp(argv[2], "-c") == 0)   //checks if the argument passed is '-c'
        edit_operations("COMM", edit, argv);
   
    else
    {
        printf("Invalid edit option!\n");
        
        fclose(edit->fptr_original);   //closing the original.mp3 file
        
        fclose(edit->fptr_mp3_edit);  //closing the duplicate.mp3 file
        
        return e_failure;
    }

    return e_success;
}

//  Editing the tag frames 
void edit_operations(char *ptr, TagEdit *edit, char *argv[])
{
    char arr[5];  //declaring a character array
    
    char flags[2];  //declaring an array to collect flags
    
    unsigned int old_size;  //declaring a variable to collect the old size
    
    char *new_cont = argv[3];  //collecting the argument in a character pointer
    
    unsigned int new_size = strlen(new_cont);  //finding the length of the new content

    for (int i = 0; i < 6; i++)
    {
        fread(arr, 4, 1, edit->fptr_original); //reads 4 bytes of tag from original.mp3
        arr[4] = '\0';

        old_size = read_big_endian(edit->fptr_original); //the converted big endian data is collected
        
        fread(flags, 2, 1, edit->fptr_original); //reads 2 bytes of flag from original.mp3

        if (strcmp(ptr, arr) == 0)  //comparing whether the argument passed is of the corresponding tag
        {   
            printf("Editing tag: %s\n", arr);  //prints the tag which we are editing

            fwrite(arr, 4, 1, edit->fptr_mp3_edit); //writes the 4 bytes of tag into duplicate.mp3
            
            write_big_endian(edit->fptr_mp3_edit, new_size);  //function call to convert the integer data to big endian
            
            fwrite(flags, 1, 2, edit->fptr_mp3_edit); //writes the 2 bytes of flag into duplicate.mp3
            
            fwrite(new_cont, 1, new_size, edit->fptr_mp3_edit); //writes the converted integer data into duplicate.mp3

            // Skipping old tag content
            fseek(edit->fptr_original, old_size, SEEK_CUR);
        }
        else
        {
            fwrite(arr, 4, 1, edit->fptr_mp3_edit);  //writes the 4 bytes of tag from original.mp3 into duplicate.mp
           
            write_big_endian(edit->fptr_mp3_edit, old_size); //writes the converted integer data into duplicate.mp3
            
            fwrite(flags, 1, 2, edit->fptr_mp3_edit);   //writes the 2 bytes of flag from original.mp3 into duplicate.mp3

            char buffer[70];
            
            fread(buffer, 1, old_size, edit->fptr_original);  //reads the old size from original.mp3
            
            fwrite(buffer, 1, old_size, edit->fptr_mp3_edit); //writes the old size into duplicate.mp3
           
        }
    }

    //  Copy remaining audio data 
    char ch;
    
    while (fread(&ch, 1, 1,  edit -> fptr_original) == 1) //reading character by character from the source
    {
        fwrite(&ch, 1, 1, edit -> fptr_mp3_edit); //writing those characters into the destination
    }


    fclose(edit->fptr_mp3_edit);   //closing the duplicate file
   
    fclose(edit->fptr_original);  //closing the original file

        
    remove(edit->mp3_edit_fname); //removes original.mp3
    rename("duplicate.mp3", edit->mp3_edit_fname);  //renames original.mp3 into duplicate.mp3
    
}