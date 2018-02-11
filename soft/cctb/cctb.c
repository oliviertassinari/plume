#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define LINE_SIZE 100

uint8_t atohex(char *s);
uint8_t getVal(char c);

int main(int argc, char *argv[]){
        FILE *f=NULL;
        FILE *g=NULL;
        int n;
        char line[LINE_SIZE];
        char address[LINE_SIZE];
        char reg[LINE_SIZE];
        char value[LINE_SIZE];
	char cmd;
        if(argc!=3){
                printf("Usage cctb codec_cfg output\n");
                return EXIT_FAILURE;
        }

        if((f=fopen(argv[1],"r"))==NULL)
                return EXIT_FAILURE;
        if((g=fopen(argv[2],"w"))==NULL)
                return EXIT_FAILURE;
        while(fgets(line,LINE_SIZE,f)!=NULL){
                for(int i=0;i<LINE_SIZE&&line[i]!='\0';i++)
                        if(line[i]==' ')
                                line[i]=',';
                n=sscanf(line,"%c,%[^,],%[^,],%s",&cmd,address,reg,value);
		if((cmd=='r')||(cmd=='w'))
		  if(n==4){
		    printf("%s %s %s ",address,reg,value);
		    (void)address; //I2C address is not needed because we use spi
		    fputc((atohex(reg)<<1)+(cmd=='r'),g);
		    fputc(atohex(value),g);
		  }
        }
        fclose(f);
        fclose(g);
        printf("\n");
        return EXIT_SUCCESS;
}

uint8_t atohex(char * s){
        return (getVal(s[0])<<4)+getVal(s[1]);        
}

uint8_t getVal(char c)
{
        int rtVal = 0;

        if(c >= '0' && c <= '9')
        {
                rtVal = c - '0';
        }
        else
        {
                rtVal = c - 'a' + 10;
        }

        return rtVal;
}
