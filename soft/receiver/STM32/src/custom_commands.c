#include "custom_commands.h"
#include <stdlib.h>
#include "chprintf.h"
#include "codec.h"
#include "communication.h"
static uint8_t atohex(char *s);
static uint8_t getVal(char c);

void cmd_reset_codecs(BaseSequentialStream *chp, int argc, char *argv[]){
    (void)argc;
    (void)argv;
    chprintf(chp,"Resetting codecs...\r\n");
    codec_send_config();
}

void cmd_set_amp(BaseSequentialStream *chp, int argc, char *argv[]){
	if(argc==3){
		uint32_t codec = atoi(argv[0]);
		uint32_t lna = atoi(argv[1]);
		uint32_t pga = atoi(argv[3]);
		codec_set_amp(codec,lna,pga);
	}else
		chprintf(chp,"Usage: set_amp codec lna pga\r\n");
}

void cmd_get_amp(BaseSequentialStream *chp, int argc, char *argv[]){
	if(argc==1){
		uint32_t codec = atoi(argv[0]);
		uint32_t lna;
		uint32_t pga;
		codec_get_amp(codec,&lna,&pga);
		chprintf(chp,"Amplification settings of codec %d:\r\n",codec);
		chprintf(chp,"LNA: %d\r\nPGA: %d\r\n",lna,pga);
	}else
		chprintf(chp,"Usage: set_amp codec amp\r\n");
	
}
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


void cmd_codecwrite(BaseSequentialStream *chp, int argc, char *argv[]){
    if(argc!=3){
	chprintf(chp,"Usage: codec_write codec_num reg val\r\n");
	return;
    }
    uint32_t codec = strtol(argv[0], NULL, 16);
    uint8_t reg = strtol(argv[1], NULL, 16);
    uint8_t val = strtol(argv[2], NULL, 16);

    codec_write(codec,reg,val);
}

void cmd_codecread(BaseSequentialStream *chp, int argc, char *argv[]){
    if(argc!=2){
	chprintf(chp,"Usage: codec_read codec_num reg\r\n");
	return;
    }
    uint32_t codec = strtol(argv[0], NULL, 16);
    uint8_t reg = strtol(argv[1], NULL, 16);

    uint8_t val = codec_read(codec,reg);
    
    chprintf(chp,"value: %x\r\n",val);
}
void cmd_startmeasure(BaseSequentialStream *chp, int argc, char *argv[]){
    (void) chp;
    (void) argc;
    (void) argv;

    com_put_event(CMD_ON);

}

void cmd_stopmeasure(BaseSequentialStream *chp, int argc, char *argv[]){
    (void) chp;
    (void) argc;
    (void) argv;

    com_put_event(CMD_OFF);

}
