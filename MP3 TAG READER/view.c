
#include <stdio.h>
#include <string.h>
#include "view.h"
#include "types.h"

/* Function definition of the function to perform help operation */

void help_option() 
{
    printf("<---------HELP--------->\n");  
    
    printf("USAGE:\n");

    printf("\t To view please pass like: ./a.out -v mp3filename\n");

    printf("\t To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");

    printf("\t To get help pass like: ./a.out --help\n");

    printf("<------------------------------------>\n");
    
    printf(" -t\tModifies a Title tag\n");

    printf(" -a\tModifies an Artist tag\n");

    printf(" -A\tModifies an Album tag\n");

    printf(" -m\tModifies the Genre tag\n");

    printf(" -y\tModifies a Year tag\n");

    printf(" -c\tModifies a Comment tag\n");

}

/* Read and Validate the view operation arguments */
Status read_and_validate_view_arg(char *argv[], TagView *view)
 {
    if(strstr(argv[2], ".mp3") != NULL)  //checks if .mp3 extension is present or not
    {
        printf(".mp3 is PRESENT\n");
        view -> mp3_view_fname = argv[2];  //collecting the .mp3 file in another structure variable
    }
    else
    {
        printf("ERROR ----> The .mp3 extension is not proper....!\n");
        return e_failure;
    }
    
    return e_success;
 }

/* Function definition of the function to perform view operation */
Status view_option(TagView *view)
 {

    if(open_files(view) == e_success)   //checks if all files are opened or not
    {
        printf("All files are opened successfully\n");
    }
    else
    {
        printf("Files are not opened successfully\n");
        return e_failure;
    }

    if(view_operation(view) == e_success) //checks if view operation is success or not
    {
        printf("....AUDIO DETAILS DISPLAYED SUCCESSFULLY....\n");
    }
    else
    {
        printf("Displaying Audio Details UNSUCCESSFULLY\n");
        return e_failure;
    }

    return e_success;
 }  

 /* Function definition of the function to open files */
 Status open_files(TagView *view)
 {
    view -> fptr_mp3_view = fopen(view -> mp3_view_fname, "r"); //opens the mp3 file in read mode
    
    if(view -> fptr_mp3_view == NULL) //checks if mp3 file is present or not
    {
        printf("Source file is not present\n");
        return e_failure;
    }
    else
    {
        printf("Source file is opened\n");
    }

    return e_success;
 }   

 /* Function definition of the function to perform view operation */
 Status view_operation(TagView *view)
 {
    fseek(view -> fptr_mp3_view, 10, SEEK_SET); //makes the file pointer to point after the header bytes
    int i;

    for(i = 0; i < 6; i++)
    {
        fetch_tag(view);  //function call to fetch & display tags

        content_size(view);  //function call to calculate content size

        content(view);  //function call to fetch & display content

    }

    return e_success;
 }

 /* Function definition of the function to Fetch Tags */
 void fetch_tag(TagView *view)
 {
    char array[10];     //declaring an array to collect the tags
    fread(array, 4, 1, view -> fptr_mp3_view);  //reads 4 bytes of tag from original.mp3
    array[4] = '\0';

    printf("%s------->",array);    //prints the tag
 }


 /* Function definition of the function to fetch the content size*/
 void content_size(TagView *view)
 {
    int size = 4;
    unsigned char num[size];    //declaring an array to collect the tag size

    fread(num, 4, 1, view -> fptr_mp3_view);   //reads 4 bytes from original.mp3

    /* Converting Big Endian to Little Endian */
    char *ptr = (char *)num;
    int i = 0;
    int j = size - 1;
    while(i < j)
    {
        int temp = ptr[i];
        ptr[i] = ptr[j];
        ptr[j] = temp;
        i++;
        j--;
    }
        unsigned int res = (num[0]) | (num[1] << 8) | (num[2] << 16) | (num[3] << 24); //converting the little endian data to integer

        view -> content_size = res;   //stores the converted data 
        fseek(view -> fptr_mp3_view, 2, SEEK_CUR);   //skips the 2 bytes of flag

 }


 /* Function definition of the function to fetch the content*/
void content(TagView *view)
{
    char arr[view -> content_size + 1];  //declaring an array too collect the content
   
    fread(arr, view -> content_size, 1, view -> fptr_mp3_view);  //reads the content from original.mp3
    
    arr[view -> content_size] = '\0';

    for(int i = 0; i < view -> content_size; i++)
    {
        printf("%c", arr[i]);  //prints the content
    }
    printf("\n");

}


/* Function definition of the function to close file*/
void file_close_view(TagView *view)
{
    fclose(view -> fptr_mp3_view);  //closes the original.mp3
}
