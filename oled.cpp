#include<Wire.h>
#include<avr/io.h>

// Definations 
#define OLED_ADDRESS 0x3C
#define OLED_CTRL_CMD_STREAM 0x00 
#define OLED_CTRL_CMD_SINGLE 0x80 // 0100 0000b --> Co D/C#
#define OLED_CTRL_DATA_STREAM 0x40 // 1000 0000b

//intialization 
#define OLED_MUX_RATIO 0XA8
#define OLED_DISPLAY_OFFSET 0XD3
#define OLED_DISPLAY_START_LINE 0X40
#define OLED_SET_SEGMENT_REMAP 0XA0
#define OLED_COM_OUTPUT_SCAN_DIRECTION 0XC0
#define OLED_COM_PINS_HARDWARE_CONFIG 0XDA
#define OLED_SET_CONTRAST_CTRL 0X81
#define OLED_DISABLE_DISPLAY_ON 0XA4
#define OLED_NORMAL_DISPLAY 0XA6
#define OLED_OSC_FREQ 0XD5
#define OLED_CHARGE_PUMP 0X8D
#define OLED_DISPLAY_ON 0xAF

#define OLED_PAGE_ADDRESSING_MODE  0x02

int main(void){
  init();
  oledBegin();
  setCursor(0, 0);
  writeData(B11001100);
  writeData(B00110011);
  writeData(0b11001100);
  writeData(0b00110011);
  return 0;
}

void oledBegin(){

  Wire.begin();
  turnOn();
  normalDisplayMode();
  setPageMode();
  writeCommand(OLED_CHARGE_PUMP);
  writeCommand(0x14);
  clearDisplay();

}

void writeCommand(byte command){
  Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(OLED_CTRL_CMD_SINGLE);
  Wire.write(command);
  Wire.endTransmission();
}

void writeData(byte data){
  Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(OLED_CTRL_DATA_STREAM);
  Wire.write(data);
  Wire.endTransmission();
}

void turnOn(){
  writeCommand(OLED_DISPLAY_ON);
}

void normalDisplayMode(){
  writeCommand(OLED_NORMAL_DISPLAY);
}

void setPageMode(){
  byte addressingMode = OLED_PAGE_ADDRESSING_MODE;
  writeCommand(0x20);        //set addressing mode
  writeCommand(OLED_PAGE_ADDRESSING_MODE);   //set page addressing mode
}

void clearDisplay(){
   for(byte page = 0; page < 8; page++){
     setCursor(0, page);
     for(byte seg = 0; seg < 128; seg++){
       writeData(0x00);
     }
   }
   setCursor(0,0);
}

void setCursor(byte X, byte Y){
  writeCommand(0x00 + (X & 0x0F));
  writeCommand(0x10 + ((X >> 4) & 0x0F)); //set column higher address
  writeCommand(0xB0 + Y);                 //set page address
}
