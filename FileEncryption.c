#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Prototypes
void XOR(char *org, char *dest, char *mpass, int size);

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("Usage: fe -m -f -l \n-m Masterpassword\n-f File to encrypt/decrypt\n-l Name of the encrypted file\n");
        return 1;
    }
    FILE *f;
    char *mpass = NULL, *org = NULL, *dest = NULL;
    int size = 0;
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                case 'm':
                    mpass = malloc(strlen(argv[i+1]) + 1);
                    strcpy(mpass, argv[i+1]);
                    while(mpass[size] != '\0')
                    {
                        mpass[size] = tolower(mpass[size]);
                        size++;
                    }
                    break;
                case 'f':
                    org = malloc(strlen(argv[i+1]) + 1);
                    strcpy(org, argv[i+1]);
                    f = fopen(org, "r");
                    if(f == NULL)
                    {
                        printf("Couldnt open the file!\n");
                        return 1;
                    }
                    fclose(f);
                    break;
                case 'l':
                    dest = malloc(strlen(argv[i+1]) + 1);
                    strcpy(dest, argv[i+1]);
                    break;
            }
        }
    }
    XOR(org, dest, mpass, size);
    free(mpass);
    free(org);
    free(dest);

}

/*
    This is the encryption/decryption function.
    It takes as input:
    1. String name of the original file
    2. String of the destination file NAME
    3. String of the masterpassword
    4. Int size of the masterpassword 


    This function performs an XOR binary operation on each byte of the original file and outputs it to the destination file name (if specified).
    If the file type is .dat, the program assumes this is a previously encrypted file and will attempt to decrypt it.
    Every non .dat file will contain a small header which will determine the file type

*/

void XOR(char *org, char *dest, char *mpass, int size)
{
    unsigned char byte;
    char *destinationFile, *ext;
    ext = strstr(org, ".");
    if(ext != NULL) // Extension exists
    {
        if(strcmp(ext, ".dat") == NULL) // Wasn't encrypted previously
        {
            if(strcmp(dest, NULL)) dest = org; // If destination wasn't specified...
            sprintf(destinationFile, "%s.dat", dest);
            
            // Encrypt the file type in the header
            int extSize = strlen(ext);
            char *header = NULL;
            header[0] = extSize;
            for(int i = 1; i < extSize; i++)
            {
                sprintf(header, "%s%s", header, ext[i]);
            }
            FILE *d = fopen(destinationFile, "w");
            fwrite(d, sizeof(int), 1, extSize^mpass[0]);
            for(int i = 0; i < strlen(header); i++)
            {
                fwrite(d, sizeof(char), 1, header[i]^ext[i+1]);
            }
            fclose(d);
            d = NULL;
        }
        else // Was encrypted
        {
            FILE *d = fopen(destinationFile, "r");
            int extSize = 0;
            char *header = NULL;
            if(strcmp(dest, NULL)) dest = org;
            sprintf(destinationFile, "%s.%s", dest, ext);
        }
    }
    else destinationFile = dest;
    
    FILE *d = fopen(destinationFile, "a");
    int i = 0;
    while(fread(org, 1, 1, byte))
    {
        fwrite(d, 1, 1, byte^mpass[i]);
        if(i == size) i = 0;
        else i++;
    }
    printf("Encrypted the file and stored it as %s", destinationFile);
}
