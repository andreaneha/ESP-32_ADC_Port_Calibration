/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include <sys/time.h>
#define DEFAULT_VREF    1100 

//funtions used
uint32_t * ADCread();
void configADC();

//constants used in files

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC1_CHANNEL_0;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;



void configADC(){
        adc_channel_t currentChannel = 0;
        for(int i = 0; i<8; i++){
            adc1_config_width(ADC_WIDTH_BIT_12);
            adc1_config_channel_atten(currentChannel,ADC_ATTEN_DB_0);
            currentChannel++;
        }
}

uint32_t * ADCread(){
    uint32_t* sensorReadings = malloc(sizeof(uint32_t)*8);
    adc_channel_t currentChannel = channel;
    for(int i = 0; i<7; i++){
        uint32_t adc_reading = 0;
        //adc1_config_width(ADC_WIDTH_BIT_12);
        //adc1_config_channel_atten(currentChannel,ADC_ATTEN_DB_0);
        //int val = adc1_get_raw(ADC1_CHANNEL_0); 
        //charecterize ADC
/*         esp_adc_cal_characteristics_t characteristics;
    esp_adc_cal_get_characteristics(DEFAULT_VREF, atten, ADC_WIDTH_10Bit, &characteristics);
    uint32_t voltage = adc1_to_voltage(currentChannel, &characteristics);
    printf("%d mV\n",voltage); */
        
        
        adc_reading = adc1_get_raw((adc1_channel_t)currentChannel);
        adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
        esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
        //esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
        //printf("(%d) Raw: %d\tVoltage: %dmV\n",currentChannel, adc_reading, voltage);
        sensorReadings[i] = voltage;
        currentChannel++;
    
        //printf(">>ADC reading: %d\n", val);
        free(adc_chars);
        }
        sensorReadings[7] = '\0';
        return sensorReadings;
}



void app_main()
{
    configADC();
    struct timeval startTime;
    struct timeval endTime;
    struct timeval hundoEnd;
    struct timeval hundoStart;
    uint32_t * readings;
    int loop = 0;
    gettimeofday(&hundoStart,NULL);
    while(1){
/*         
        printf("Hello world!\n");

    //Print chip information 
        esp_chip_info_t chip_info;
        esp_chip_info(&chip_info);
        printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
                chip_info.cores,
                (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
                (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : ""); 

        printf("silicon revision %d, ", chip_info.revision);

        printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");*/


            if (loop < 100){
                gettimeofday(&startTime,NULL);
                readings = ADCread();

                gettimeofday(&endTime,NULL);

                long elapsed = (endTime.tv_sec-startTime.tv_sec)*1000000LL + endTime.tv_usec-startTime.tv_usec;
                long start = startTime.tv_sec*1000000LL + startTime.tv_usec;
                long end = endTime.tv_sec*1000000LL + startTime.tv_usec;
        //fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%lu,%lu,%lu\n",
         //readings[0], readings[1], readings[2], readings[3], readings[4], readings[5], readings[6], readings[7],
         //   start, end, elapsed);

                printf("%d,%d,%d,%d,%d,%d,%d,%lu,%lu,%lu\n",
                readings[0], readings[1], readings[2], readings[3], readings[4], readings[5], readings[6],
                start, end, elapsed);
        
                free(readings);
                
            }
            
            if(loop == 100){
                gettimeofday(&hundoEnd,NULL);
                long elapsed = (hundoEnd.tv_sec-hundoStart.tv_sec)*1000000LL + hundoEnd.tv_usec-hundoStart.tv_usec;  
                printf("100 readings taken in %lu microSeconds\n", elapsed);
            }
            loop++;
            fflush(stdout);
        }
}
