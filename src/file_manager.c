#include <stdio.h>
void create_file(const char* filename)
{
    FILE *file;
    file = fopen(filename, "a+");
    
    if(file == NULL)
    {
        printf("Failed to create file\n");
    }
}