#include "Arduino.h"
#include "MIDI.h"

const int table [6][9] = {
  {1,7,19,49,43,37,31,25,13},{2,8,20,-1,44,38,26,14},{3,9,21,-1,45,39,27,15},{4,10,22,-1,46,40,28,16},
  {5,11,23,-1,47,41,29,17},{6,12,24,-1,48,42,30,18}
  };
 
//These will be the output pins (black pins) maybe the other way around
//const int pin_o [9] = {8,9,10,11,12,14,15,16,17};
const int pin_o [6] = {14,15,16,17,12,11};

//These will be the input pins (red pins)
const int pin_i [9] = {2,3,10,4,9,8,7,6,5};

//These will save if note is on or off
bool note_state[49] = {false};

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();


void setup()
{
  for(int i = 0; i < 6; i++)
  {
    pinMode(pin_o[i], OUTPUT);
  }

  for(int i = 0; i < 9; i++)
  {
    pinMode(pin_i[i], INPUT_PULLUP);
  }
  pinMode(18,OUTPUT);
  //MIDI.begin(MIDI_CHANNEL_OFF);
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Goodnight moon!");

  
}

void niba(int note, bool state)
{
  if(state)
  {
    double power = ((double)note-34)/12;
    tone(18, 440*pow(2, power));
  }

  else
  {
    noTone(18);
  }
 
}
void loop() {
  
  int note = 0;
  for(int o = 0; o < 6; o++)
  {
    //cycle through output pins and send GND signal
    digitalWrite(pin_o[o], LOW);

    //repad input signals for GND, meaning made connection
    for(int i = 0; i < 9; i++)
    {

       if(i == 8 && o>0)
       {
        continue;
       }

       //Serial.println(o);
       //Serial.println(i);
       //Serial.println('\n');


      note = 6*i+1+o;
      if(digitalRead(pin_i[i])==LOW && !note_state[note-1])
      {

        //Serial.println(table[i][o]);
        Serial.println(note);
        //niba(note, 1);
        //this is .sendNoteOn(number of note, volume, midi channel)
        //MIDI.sendNoteOn(note+35, 127, 1);
        note_state[note-1] = 1;
      }
      
      else if(digitalRead(pin_i[i])==HIGH && note_state[note-1])
      {
        //Serial.println(note_state[note-1]);
      
        note_state[note-1] = 0;
        Serial.println(note);
        //niba(note, 0);
        //MIDI.sendNoteOff(note+35, 0, 1);
      }
    }
    digitalWrite(pin_o[o], HIGH);
  }
  
  
}
