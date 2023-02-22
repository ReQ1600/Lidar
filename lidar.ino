#include "Seeed_vl53l0x.h"

Seeed_vl53l0x VL53L0X;
//next send data to phone app through bluetooth

void setup()
{
  VL53L0X_Error Status = VL53L0X_ERROR_NONE;
  Serial.begin(9600);
  Status = VL53L0X.VL53L0X_common_init();
  if(Status != VL53L0X_ERROR_NONE)
  {
    Serial.println("vl failed to start");
    Serial.println(Status);
    while(true);
  }
  VL53L0X.VL53L0X_single_ranging_init();
  if (Status != VL53L0X_ERROR_NONE)
  {
    Serial.println("vl failed to start");
    Serial.println(Status);
    while(true);
  }
}

VL53L0X_RangingMeasurementData_t Data;
VL53L0X_Error Status = VL53L0X_ERROR_NONE;

int Measurements[14];
int FilteredMeasurement = 0;

void loop() 
{

  for(int i = 0; i<14; i++)//performing 14 measurments
  {
    memset(&Data, 0, sizeof(VL53L0X_RangingMeasurementData_t));
    Status = VL53L0X.PerformSingleRangingMeasurement(&Data);
  
    if(Status == VL53L0X_ERROR_NONE)
    {
      if(Data.RangeMilliMeter >= 2000)
      {
        Serial.println("out of range");
        Measurements[i] = 2011;
      }
      else
      {
        Measurements[i] = Data.RangeMilliMeter;
      }
    }
    else
    {
      Serial.println("measurment failed!");
      Serial.println(Status);
    }
    delay(100);//might be smaller
  }

  qsort(Measurements, 14, sizeof(int), [](const void *m1, const void* m2){return *(int*)m1 - *(int*)m2;});//must be filtered cuz it's error is between 0-5mm which is a lot. using qsort cuz i dont think arduino has std::sort
  for(int i = 0; i < 14; i++)
  {
    if(i > 2 && i < 7) FilteredMeasurement+=Measurements[i];//taking more from the bottom cuz sensor has tendency to return larger values than actual val 
  }
  FilteredMeasurement = FilteredMeasurement / 4 - 10;
  Serial.println(FilteredMeasurement);
  FilteredMeasurement = 0;
  delay(2000);//so i can see
}
