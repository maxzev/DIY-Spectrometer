#ifndef SPECTROMETER_H_
#define SPECTROMETER_H_

#include "delay.h"

#define SPEC_VIDEO_ADC hadc1
extern ADC_HandleTypeDef SPEC_VIDEO_ADC;

void spectrometerInit();
void runSpectrometer();

#endif /* SPECTROMETER_H_ */
