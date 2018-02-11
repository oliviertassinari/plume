#ifndef CALIBRATION_DB_H
#define CALIBRATION_DB_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <jsoncpp/json/json.h>

using namespace std;

class CalibrationDB{
    public:
        CalibrationDB(const char *const jsonFile);
        ~CalibrationDB();

        void getCalibrationEmitter(double dst[3], const char * const _mac);
        void getCalibrationReceiver(double dst[6], const char _mac[6]);

    private:
        Json::Value root;   
        Json::Reader reader;

        void convertToHex(char *dst, const char *const src, const int size);
        char hexFromValue(const char c);
};

#endif
