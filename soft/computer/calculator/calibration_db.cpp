#include "calibration_db.h"

CalibrationDB::CalibrationDB(const char *const jsonFile){
    ifstream document(jsonFile, ifstream::in);
    if(!(reader.parse(document, root, false))){
        printf("Failed json parse.");
        exit(1);
    }
}

CalibrationDB::~CalibrationDB(){
}

void CalibrationDB::getCalibrationEmitter(double dst[3], const char * const _mac){
    for(int i=0;i<3;i++)
        dst[i] = root["calibration"]["emitters"][_mac].get(i,1.0).asDouble();
}

void CalibrationDB::getCalibrationReceiver(double dst[6], const char _mac[6]){
    char hex_mac[13];
    convertToHex(hex_mac,_mac,6);
    for(int i=0;i<6;i++)
        dst[i] = root["calibration"]["receivers"][hex_mac].get(i,1.0).asDouble();
}

void CalibrationDB::convertToHex(char *dst, const char *const src, const int size){
    for(int i=0;i<size;i++){
        dst[2*i]=hexFromValue((src[i]&0xf0)>>4); // High
        dst[2*i+1]=hexFromValue(src[i]&0x0f); // Low
    }
    dst[2*size] = '\0';

    //printf("%s\n", dst);
}

char CalibrationDB::hexFromValue(const char c){
    if(c<10)
        return c+'0';
    else
        return c+'a'-10;
}
