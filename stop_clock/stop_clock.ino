// Clock, written by  Tm4n (check Github)
// using a DS1302 clock module, a tm1637 display driver and two keys for setting the time
// displays humidity and temperature, too

#include <EEPROM.h>
#include <Time.h>

#include "TM1637.h"
#define CLK 3 //pins definitions for TM1637 and can be changed to other ports       
#define DIO 2

TM1637 tm1637(CLK,DIO);

#include <stdio.h>
#include <DS1302.h>

const int kCePin   = 7;  // Chip Enable/Rst
const int kIoPin   = 8;  // Input/Output
const int kSclkPin = 9;  // Serial Clock

// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin);

#include "Button.h"

// own stuff
int mode = 0;
int brightness = BRIGHT_TYPICAL;

const int pinButtonSel = 11;
const int pinButtonSet = 12;

Button ButtonSel(pinButtonSel, BUTTON_PULLUP_INTERNAL);
Button ButtonSet(pinButtonSet, BUTTON_PULLUP_INTERNAL);


void setup()
{
  // load values from EEPROM
  brightness = EEPROM.read(0);
  if (brightness > 7) brightness = BRIGHT_TYPICAL;
  
  tm1637.init();
  tm1637.set(brightness);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  // Initialize a new chip by turning off write protection and clearing the
  // clock halt flag. These methods needn't always be called. See the DS1302
  // datasheet for details.
  rtc.writeProtect(false);
  rtc.halt(false);

  // Make a new time object to set the date and time.
  // Sunday, September 22, 2013 at 01:38:50.
  //nTime t(2014, 6, 12, 22, 55, 50, nTime::kThursday);

  // Set the time and date on the chip.
  //rtc.time(t);
}

int setmode;

void settings()
{
  // set clock
  
  setmode = 0;
  nTime tm = rtc.time(); // get current time
  tm.sec = 0;
  int h1 = ((int)tm.hr) % 10;
  int h2 = (((int)tm.hr)/10) % 10;
      
  int m1 = ((int)tm.min) % 10;
  int m2 = (((int)tm.min)/10) % 10;
  
  int blk = -150;
  while(setmode < 4)
  {
    if (blk >= 150) blk = -150;
    if (blk == -150)
    {
      tm1637.point(true);
      tm1637.display(0,h2);
      tm1637.display(1,h1); 
      tm1637.display(2,m2);
      tm1637.display(3,m1);
    }
    if (blk == 0)
    {
      tm1637.point(true);
      if (setmode == 0) tm1637.display(0,18); else tm1637.display(0,h2);
      if (setmode == 1) tm1637.display(1,18); else tm1637.display(1,h1);
      if (setmode == 2) tm1637.display(2,18); else tm1637.display(2,m2);
      if (setmode == 3) tm1637.display(3,18); else tm1637.display(3,m1);
    }
    switch (setmode)
    {
      case 0:
        if (ButtonSel.uniquePress()) h2++;
        if (h2 > 2) h2 = 0;
      break;
      case 1:
        if (ButtonSel.uniquePress()) h1++;
        if (h1 > 3 && h2 == 2) h1 = 0;
        if (h1 > 9) h1 = 0;
      break;
      case 2:
        if (ButtonSel.uniquePress()) m2++;
        if (m2 > 5) m2 = 0;
      break;
      case 3:
        if (ButtonSel.uniquePress()) m1++;
        if (m1 > 9) m1 = 0;
      break;
    }
    if (ButtonSet.uniquePress()) setmode++;
    delay(1);
    blk++;
  }
  
  tm.sec = 0;
  tm.min = m2*10 + m1;
  tm.hr = h2*10 + h1;
  
  // upload time to chip
  rtc.time(tm);
  
  
  // set date
  setmode = 0;
  tm1637.point(false);
  tm1637.display(2,18);
  tm1637.display(3,13);
  
  int d1 = ((int)tm.date) % 10;
  int d2 = (((int)tm.date)/10) % 10;
  
  while (setmode < 2)
  {
    if (blk >= 150) blk = -150;
    if (blk == -150)
    {
      tm1637.display(0,d2);
      tm1637.display(1,d1); 

    }
    if (blk == 0)
    {
      if (setmode == 0) tm1637.display(0,18); else tm1637.display(0,d2);
      if (setmode == 1) tm1637.display(1,18); else tm1637.display(1,d1);
    }
    switch (setmode)
    {
      case 0:
        if (ButtonSel.uniquePress()) d2++;
        if (d2 > 3) d2 = 0;
      break;
      case 1:
        if (ButtonSel.uniquePress()) d1++;
        if (d1 > 1 && d2 == 3) d1 = 0;
        if (d1 > 9) if (d2 == 0) d1 = 1; else d1 = 0;
      break;
    }
    if (ButtonSet.uniquePress()) setmode++;
    delay(1);
    blk++;
  }
  tm.date = d2*10 + d1;
  if (tm.date == 0) tm.date = 1;
  
  
  // set month
  setmode = 0;
  tm1637.point(false);
  tm1637.display(2,22);
  tm1637.display(3,23);
  
  int m = (int)tm.mon;
  
  while (setmode < 1)
  {
    if (blk >= 150) blk = -150;
    if (blk == -150)
    {
      if (m < 10) tm1637.display(0,18); else tm1637.display(0, (m/10)%10);
      tm1637.display(1, m%10); 

    }
    if (blk == 0)
    {
      tm1637.display(0,18);
      tm1637.display(1,18);
    }
    
    if (ButtonSel.uniquePress()) m++;
    if (m > 12) m = 1;
      
    if (ButtonSet.uniquePress()) setmode++;
    delay(1);
    blk++;
  }
  tm.mon = m;
  
  
  // set year
  setmode = 0;
  tm1637.point(false);
  tm1637.display(0,2);
  tm1637.display(1,0);
  
  int y1 = ((int)tm.yr) % 10;
  int y2 = (((int)tm.yr)/10) % 10;
  
  while (setmode < 2)
  {
    if (blk >= 150) blk = -150;
    if (blk == -150)
    {
      tm1637.display(2,y2);
      tm1637.display(3,y1); 

    }
    if (blk == 0)
    {
      if (setmode == 0) tm1637.display(2,18); else tm1637.display(2,y2);
      if (setmode == 1) tm1637.display(3,18); else tm1637.display(3,y1);
    }
    switch (setmode)
    {
      case 0:
        if (ButtonSel.uniquePress()) y2++;
        if (y2 > 9) y2 = 0;
      break;
      case 1:
        if (ButtonSel.uniquePress()) y1++;
        if (y1 > 9) y1 = 0;
      break;
    }
    if (ButtonSet.uniquePress()) setmode++;
    delay(1);
    blk++;
  }
  tm.yr = 2000 + y2*10 + y1;
  
  // upload date to chip
  rtc.year(tm.yr);
  rtc.month(tm.mon);
  rtc.date(tm.date);
  
  
  
  
  
  /////////////////////////////////////////
  // brightness setting
  
  setmode = 0;
  blk = -150;
  while (setmode == 0)
  {
    if (blk >= 150) blk = -150;
    if (blk == -150)
    {
      tm1637.point(false);
      tm1637.display(0,18);
      tm1637.display(1,18); 
      tm1637.display(2,brightness);
      tm1637.display(3,11);
    }
    if (blk == 0)
    {
      tm1637.display(2,18);
    }
    if (ButtonSel.uniquePress())
    {
      brightness++;
      if (brightness > 7) brightness = 0;
      tm1637.set(brightness);
    }
    
    if (ButtonSet.uniquePress()) setmode++;
    delay(1);
    blk++;
  }
  // safe brightness to EEPROM
  EEPROM.write(0, brightness);
  
  mode = 0;
}







int8_t digitdata[4];

void loop() {
  float h, t;
  switch(mode)
  {
    case 0:
    {
      // Display current time
      nTime tm = rtc.time();
    
      int h1 = ((int)tm.hr) % 10;
      int h2 = (((int)tm.hr)/10) % 10;
      
      int m1 = ((int)tm.min) % 10;
      int m2 = (((int)tm.min)/10) % 10;
      
      tm1637.point(true);
      if (h2 == 0) digitdata[0] = 18; else digitdata[0] = h2;
      digitdata[1] = h1; 
      digitdata[2] = m2;
      digitdata[3] = m1;
      tm1637.display(digitdata);
    }
    break;
    
    case 1:
    {
      // Display time since press
      time_t t = now();
      
      if (t < 60)
      if (t < 3600)
      {
        // display mins and secs
        int m1 = ((int)minute()) % 10;
        int m2 = (((int)minute())/10) % 10;
        
        int s1 = ((int)second()) % 10;
        int s2 = (((int)second())/10) % 10;
        
        tm1637.point(true);
        digitdata[0] = m2;
        digitdata[1] = m1; 
        digitdata[2] = s2;
        digitdata[3] = s1;
        tm1637.display(digitdata);
      }
      else
      {
        // display hours and mins
        int h1 = ((int)hour()) % 10;
        int h2 = (((int)hour())/10) % 10;
        
        int m1 = ((int)minute()) % 10;
        int m2 = (((int)minute())/10) % 10;
        
        tm1637.point(true);
        if (h2 == 0) digitdata[0] = 18; else digitdata[0] = h2;
        digitdata[1] = h1; 
        digitdata[2] = m2;
        digitdata[3] = m1;
        tm1637.display(digitdata);
      }
    }
    break;
    
  }

  unsigned int i;
  unsigned updt;
  if (mode == 1) updt = 2; else updt = 500;
  for (i = 0; i <= updt; i++)
  {
    // check for mode change
    if (ButtonSel.uniquePress())
    { 
      if (mode == 0)
      {
        mode = 1;
        // reset timer current time
        setTime(0);
      }
      else
     {
       mode = 0;
       delay(2000);
     }
      
      i = 509;
    }
    
    if (ButtonSet.uniquePress())
    {
      settings();
      i = 509;
    }
    delay(10);
  }
  
}
