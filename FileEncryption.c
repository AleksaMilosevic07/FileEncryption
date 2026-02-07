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
    ext = malloc(strlen(strstr(org, ".")));
    ext = strstr(org, ".");
    if(ext != NULL) // Extension exists
    {
        if(strcmp(ext, ".dat") != 0) // Wasn't encrypted previously
        {
            if(dest == NULL) dest = org; // If destination wasn't specified...
            destinationFile = malloc(sprintf(destinationFile, "%s.dat", dest));
            sprintf(destinationFile, "%s.dat", dest);
            
            // Encrypt the file type in the header
            int extSize = strlen(ext);
            char *header = malloc(extSize + 2);
            header[0] = extSize;
            for(int i = 1; i < extSize; i++)
            {
                sprintf(header + strlen(header), "%c",(char) ext[i]);
            }
            FILE *d = fopen(destinationFile, "w");
            unsigned char bb = (unsigned char)extSize^(unsigned char)mpass[0];
            fwrite(&bb, sizeof(unsigned char), 1, d);
            for(int i = 1; i < strlen(header); i++)
            {
                unsigned char byte = (unsigned char)header[i]^mpass[i];
                fwrite(&byte, sizeof(unsigned char), 1, d);
            }
            fclose(d);
            d = NULL;
        }
        else // Was encrypted
        {
            FILE *d = fopen(destinationFile, "r");
            int extSize = 0;
            char *header = NULL;
            if(dest == NULL) dest = org;
            sprintf(destinationFile, "%s.%s", dest, ext);
        }
    }
    else destinationFile = dest;
    
    FILE *d = fopen(destinationFile, "a");
    FILE *f_org = fopen(org, "r");
    int i = 0;
    while(fread(&byte, 1, 1, f_org))
    {
        byte = byte^mpass[i];
        fwrite(&byte, 1, 1, d);
        if(i == size) i = 0;
        else i++;
    }
    printf("Encrypted the file and stored it as %s", destinationFile);
    free(ext);
    free(destinationFile);
}
