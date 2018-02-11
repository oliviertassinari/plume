#ifndef MEASURE_H
#define MEASURE_H

#define MEASURES_READY 0x1

// initializes the spi, i2s and sai connections, configures the codecs through the spi connection
// emit_freq is the frequency of emission cycle (number of emission by cycle per second)
// ready is the eventsource on which to notify when the measures are done
int  measure_init(int emit_freq);
int  measure_start(int emit_freq);
void measure_stop(void);
void measure_set_calib(float* new_calib);
int  set_emit_frequency(int emit_freq);

extern Mutex mtx_ext_measures;
extern uint32_t ext_measure[9];

#endif // MEASURE_H
