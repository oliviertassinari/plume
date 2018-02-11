#include "custom_commands.h"

static uint8_t atohex(char *s);
static uint8_t getVal(char c);

void cmd_spiread(BaseSequentialStream *chp, int argc, char *argv[]){
		int n=atohex(argv[0]);
		uint8_t value;
		if(argc!=1){
				chprintf(chp,"Usage: spiread xx\n");
				return;
		}
		for(int i=0;i<n;i++){
				spiSelect(&SPID1);
				spiReceive(&SPID1,1,&value);
				spiUnselect(&SPID1);
				chprintf(chp,"%x\n",value);
		}
}

void cmd_spiwrite(BaseSequentialStream *chp, int argc, char *argv[]){
		uint8_t value;
		if(argc==0){
				chprintf(chp,"Usage: spiread value1 ... valuen\n");
				return;
		}
		spiSelect(&SPID1);
		for(int i=0;i<argc;i++){
				value=atohex(argv[i]);
				spiSend(&SPID1,1,&value);
		}
		spiUnselect(&SPID1);

}


static uint8_t atohex(char * s){
		return (getVal(s[0])<<4)+getVal(s[1]);        
}

static uint8_t getVal(char c)
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
