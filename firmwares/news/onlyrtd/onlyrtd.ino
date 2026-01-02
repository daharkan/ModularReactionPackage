#include <SPI.h>
#include <MAX31865.h>

constexpr uint32_t BAUD = 115200;

// CS pins for the two MAX31865 boards (adjust if needed)
constexpr uint8_t CS1 = 5;
constexpr uint8_t CS2 = 2;

// PT100 sensors
MAX31865_RTD rtd1(MAX31865_RTD::RTD_PT100, CS1);
MAX31865_RTD rtd2(MAX31865_RTD::RTD_PT100, CS2);

void setup() {
  Serial.begin(BAUD);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE1);
  pinMode(10, OUTPUT); // keep SPI in master mode

  // 50Hz filter, no 3-wire, clear faults, no auto fault detect
  rtd1.configure(true, false, true, MAX31865_FAULT_DETECTION_NONE);
  rtd2.configure(true, false, true, MAX31865_FAULT_DETECTION_NONE);
}

void loop() {
  rtd1.read_all();
  rtd2.read_all();

  float t1 = rtd1.temperature();
  float t2 = rtd2.temperature();

  Serial.print("T1=");
  Serial.print(t1, 1);
  Serial.print(" C, T2=");
  Serial.print(t2, 1);
  Serial.println(" C");

  delay(1000);
}
