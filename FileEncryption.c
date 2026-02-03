#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("Usage: fe -f -l\n");
        printf("-f Encrypt/Decrypt the file\n");        
        printf("-l Destination of the encrypted file\n");
        return 1;
    }
    FILE *f;
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                case 'f':
                    f = fopen(argv[i+1], "r");
                    if(f == NULL)
                    {
                        printf("Couldnt open the file!\n");
                        return 1;
                    }
                    break;
                
                default:
                    break;
            }
        }
    }
}

void XOR(FILE *org, char *dest)
{
    unsigned char byte;
    FILE *d = fopen(dest, "w");
    while(fread(org, 1, 1, byte))
    {
        fwrite(d, 1, 1, byte^1111);
    }
    printf("Done!");
}
