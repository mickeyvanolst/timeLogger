// TIME LOGGER

#include <SD.h>

// SD variables
const int chipSelect = 4;

// LED variables
const int rLed = 5;
const int gLed = 9;
const int yLed = 6;

// BUTTON variables
const int rBtn = 8;
const int gBtn = 7;
const int yBtn = 2;

int rBtnState = LOW;
int gBtnState = LOW;
int yBtnState = LOW;

int rRec = LOW;
int gRec = LOW;
int yRec = LOW;

int prevPressed;
boolean isChanging = false;
int currProject = 0;

// TIMER
long prevMillis = 0;
long interval = 500;

// Storing the time before writing it to the SD card
int tempSecond;
int tempMinute;
int tempHour;
int tempDay;
int tempMonth;
int tempYear;

int prevTempSecond;
int prevTempMinute;
int prevTempHour;
int prevTempDay;
int prevTempMonth;
int prevTempYear;

// string that goes into the SD
String writeThis = "";

void setup() {
  timeModSetup(); // i'm to lazy to turn this into a class so it's just compiling as a function

  // pinmodes for LED
  pinMode(rLed, OUTPUT);
  pinMode(gLed, OUTPUT);
  pinMode(yLed, OUTPUT);

  // pinmodes for BUTTONS
  pinMode(rBtn, INPUT);
  pinMode(gBtn, INPUT);
  pinMode(yBtn, INPUT);

  // pinmode for SD
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

}

void loop() {
  timeModLoop(); // i'm to lazy to turn this into a class so it's just compiling as a function

  unsigned long curMillis = millis();

  rBtnState = digitalRead(rBtn);
  gBtnState = digitalRead(gBtn);
  yBtnState = digitalRead(yBtn);

  if(rBtnState == HIGH && rRec == LOW && prevPressed != 1) {
    if(gRec == HIGH) {
      gRec = LOW;
      Serial.println("GREEN OFF");
      endEntry();
    }
    if(yRec == HIGH) {
      yRec = LOW;
      Serial.println("YELLOW OFF");
      endEntry();
    }
    rRec = HIGH;
    Serial.println("RED ON");
    currProject = 1;
    startEntry();
    //    if(isChanging) {
    //      updateTimeVals(); // checking time and putting tempTime into prevTempTime values
    //    } else {
    //      getDateDs1307(); // checking time and putting it into tempTime values
    //    }
  } 
  else if(rBtnState == HIGH && rRec == HIGH && prevPressed != 1) {
    rRec = LOW;
    Serial.println("RED OFF");
    endEntry();
  }

  if(gBtnState == HIGH && gRec == LOW && prevPressed != 2) {
    if(yRec == HIGH) {
      yRec = LOW;
      Serial.println("YELLOW OFF");
      endEntry();
    }
    if(rRec == HIGH) {
      rRec = LOW;
      Serial.println("RED OFF");
      endEntry();
    }
    gRec = HIGH;
    Serial.println("GREEN ON");
    currProject = 2;
    startEntry();
  } 
  else if(gBtnState == HIGH && gRec == HIGH && prevPressed != 2) {
    gRec = LOW;
    Serial.println("GREEN OFF");
    endEntry();
  }

  if(yBtnState == HIGH && yRec == LOW && prevPressed != 3) {
    if(rRec == HIGH) {
      rRec = LOW;
      Serial.println("RED OFF");
      endEntry();
    }
    if(gRec == HIGH) {
      gRec = LOW;
      Serial.println("GREEN OFF");
      endEntry();
    }
    yRec = HIGH;
    Serial.println("YELLOW ON");
    currProject = 3;
    startEntry();
  } 
  else if(yBtnState == HIGH && yRec == HIGH && prevPressed != 3) {
    yRec = LOW;
    Serial.println("YELLOW OFF");
    endEntry();
  }


  if(rRec == HIGH) {
    digitalWrite(rLed, HIGH);
  } 
  else if(rRec == LOW) {
    digitalWrite(rLed, LOW);
  }

  if(gRec == HIGH) {
    digitalWrite(gLed, HIGH);
  } 
  else if(gRec == LOW) {
    digitalWrite(gLed, LOW);
  }

  if(yRec == HIGH) {
    digitalWrite(yLed, HIGH);
  } 
  else if(yRec == LOW) {
    digitalWrite(yLed, LOW);
  }


  // timer not really used yet..
  if(curMillis - prevMillis > interval) {
    // save the last time you blinked the LED 
    prevMillis = curMillis;   

  }


  // Checking if button is not being hold for a while and not more than one button is pressed

  if(rBtnState == HIGH && gBtnState == HIGH && yBtnState == HIGH) {
    //ignore
    prevPressed = 0;
  } 
  else if(rBtnState == HIGH && gBtnState == HIGH) {
    // ignore
    prevPressed = 0;
  } 
  else if(gBtnState == HIGH && yBtnState == HIGH) {
    // ignore
    prevPressed = 0;
  } 
  else if(yBtnState == HIGH && rBtnState == HIGH) {
    // ignore
    prevPressed = 0;
  } 
  else if(rBtnState == HIGH) {
    prevPressed = 1;//red
  } 
  else if(gBtnState == HIGH) {
    prevPressed = 2;//green
  } 
  else if(yBtnState == HIGH) {
    prevPressed = 3;//yellow
  } 
  else {
    prevPressed = 0;//empty
  }
}

void updateTimeVals() {
  getDateDs1307();
  prevTempSecond = tempSecond;
  prevTempMinute = tempMinute;
  prevTempHour = tempHour;
  prevTempDay = tempDay;
  prevTempMonth = tempMonth;
  prevTempYear = tempYear;
}

void startEntry() {
    getDateDs1307();
    
    writeThis += currProject;
    writeThis += ";";
    writeThis += tempYear;
    writeThis += ";";
    writeThis += tempMonth;
    writeThis += ";";
    writeThis += tempDay;
    writeThis += ";";
    writeThis += tempHour;
    writeThis += ";";
    writeThis += tempMinute;
    writeThis += ";";
    writeThis += tempSecond;
    writeThis += "-";
    //writeToSD();
}

void endEntry() {
    getDateDs1307();
    
    writeThis += tempYear;
    writeThis += ";";
    writeThis += tempMonth;
    writeThis += ";";
    writeThis += tempDay;
    writeThis += ";";
    writeThis += tempHour;
    writeThis += ";";
    writeThis += tempMinute;
    writeThis += ";";
    writeThis += tempSecond;
    writeThis += "\n";
    writeToSD();
    
    currProject = 0;
}

void writeToSD() {
  // Write the time to SD
  /*
   each line should be like this
   
   idnr;projectnr;years;months;days;hours;minutes;seconds
   0;1;2012;4;25;11;15;57
   
   */
  Serial.println(" ");

  // make a string for assembling the data to log:
  String dataString = "";

//  dataString += "projectnr";
//  dataString += ";";
//  dataString += "year";
//  dataString += ";";
//  dataString += "month";
//  dataString += ";";
//  dataString += "day";
//  dataString += ";";
//  dataString += "hour";
//  dataString += ";";
//  dataString += "minute";
//  dataString += ";";
//  dataString += "second";
  dataString = writeThis;

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.print(dataString);
    
    dataString = "";
    writeThis = "";
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
}




