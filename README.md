# Multi terminal HVDC transmission line protection using Traveling Waves
This project utilizes wavelet analysis to provide accurate fault locations for multi-terminal HVDC transmission line after isolating fault transmission line from the grid by two relays at the two ends of the transmission line.This technique can provide fast fault isolation, accurate fault location and system continuity.
This project has two sides: 
1-MATLAB simulation.
2-Emedded systems prototype.
# MATLAB simulink

Check full project on Matlab [here](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/tree/main/HVDC%20Matlab)

Here is the simulink code:
```
function [tr12,tr14,tr24,tr23,tr34] = fcn(dt12,dt21,dt012,dt021,dt23,dt32,dt023,dt032,
dt14,dt41,dt014,dt041,dt24,dt42,dt024,dt042,dt34,dt43,dt034,dt043)
 tr12=1; tr14=1; tr24=1; tr23=1; tr34=1;
 
 if (abs(dt12) >=80 || abs(dt23) >=80 || abs(dt14) >=80 || abs(dt24) >=80 || abs
(dt34) >=80) % fault occurs
 
 if ((dt12 >=80 && dt21 >=80) || (dt012 <=-80 && dt021 <=-80)) % fault in 
Line 1-2
 tr12=0;
 if((dt12 >=80 && dt21 >=80) && (dt012 <=-80 && dt021 <=-80))
 disp('Line 1-2 Bipolar Fault');
 
 elseif (dt12 >=80 && dt21 >=80)
 disp('Line 1-2 +ve Pole To Ground Fault');
 
 else
 disp('Line 1-2 -VE Pole To Ground Fault');
 end
 end
 if ((dt14 >=80 && dt41 >=80) || (dt014 <=-80 && dt041 <=-80)) % fault in 
Line 1-4
 tr14=0;
 if((dt14 >=80 && dt41 >=80) && (dt014 <=-80 && dt041 <=-80))
 disp('Line 1-4 Bipolar Fault');
 
 elseif (dt14 >=80 && dt41 >=80)
 disp('Line 1-4 +ve Pole To Ground Fault');
 
 else
 disp('Line 1-4 -VE Pole To Ground Fault');
 end
 end
 if ((dt24 >=80 && dt42 >=80) || (dt024 <=-80 && dt042 <=-80)) % fault in 
Line 2-4
 tr24=0;
 if((dt24 >=80 && dt42 >=80) && (dt024 <=-80 && dt042 <=-80))
 disp('Line 2-4 Bipolar Fault');
 
 elseif (dt24 >=80 && dt42 >=80)
 disp('Line 2-4 +ve Pole To Ground Fault');
 
 else
 disp('Line 2-4 -VE Pole To Ground Fault');
 end
 end
 if ((dt23 >=80 && dt32 >=80) || (dt023 <=-80 && dt032 <=-80)) % fault in 
Line 2-3
 tr23=0;
 if((dt23 >=80 && dt32 >=80) && (dt023 <=-80 && dt032 <=-80))
 disp('Line 2-3 Bipolar Fault');
 
 elseif (dt23 >=80 && dt32 >=80)
 disp('Line 2-3 +ve Pole To Ground Fault');
 
 else
 disp('Line 2-3 -VE Pole To Ground Fault');
 end
 end
 if ((dt34 >=80 && dt43 >=80) || (dt034 <=-80 && dt043 <=-80)) % fault in 
Line 3-4
 tr34=0;
 if((dt34 >=80 && dt43 >=80) && (dt034 <=-80 && dt043 <=-80))
 disp('Line 3-4 Bipolar Fault');
 
 elseif (dt34 >=80 && dt43 >=80)
 disp('Line 3-4 +ve Pole To Ground Fault');
 
 else
 disp('Line 3-4 -VE Pole To Ground Fault');
 end
 end
 
 end
 ```
Here is the system circuit on MATLAB 
![matlab circuit](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/MatLab/matlab%20circuit.png)

And these are the power & voltage charts at the buses at normal operation.
![power & voltage](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/MatLab/Normal%20operation/power%26voltage%20normal%20operation.jpg)

These are the Relays charts at normal operation.
![relays normal operation](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/MatLab/Normal%20operation/relays%20normal%20operation.jpg)

Feel free to check out the power & voltage , current and relays charts at fault before and after the protection process in [assets/matlab](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/tree/main/assets/MatLab) folder.

After clearing the fault, we use the wavvelet to aquire the fault location, but first we need to determine the wave velocity using the transmission line capacitance and reactance as shown below:

![wave velocity](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/MatLab/wave%20velocity.png)

Now we get the fault distance as shown:

![fault distance](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/fault%20distance.png)

# Project prototype

## Tools

* Arduino IDE
* oscilloscope

## Components

* 1 x arduino uno
* 3 x ATMEGA328
* 2 x 1 ohm shunt resistor
* 4 x 5v relay module
* 3 x pushbutton
* LED load

## Circuit diagram

![circuit](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/Prototype/prototype%20cicuit.png)

## Code

We used the one relay with ATMEGA328 & shunt resistor at on end of each T.L to contrlol the other relay at the other end of the same T.L.The use of a shunt resistor is to calculate the current through a line safely by measuring the voltage acrros the 1 ohm resistor, then we aquire the current using ohm's law (I = V / R).Since R is 1 ohm, then I=V.We measure the voltage by connecting on end of the resistor to the GROUND of the ATMEGA328 and the other end of the resistor to A0 pin (ADC) of the MCU.

### Controller MCU code :
```
float value1;
int x=0;
void setup() {
 // put your setup code here, to run once:
pinMode(13,OUTPUT); //pin 13 as output(relay)
pinMode(12,INPUT); //pin 12 as input (reset)
pinMode(11,INPUT); //pin 11 as input (arduino2 fault)
digitalWrite(13,LOW); //pin 13 is low
Serial.begin(9600); //start serial
}
void loop() {
 // put your main code here, to run repeatedly:
 value1=(analogRead(A0)*1.8180048077); //voltage across shunt resistor or current 
since R=1 // 4882.8125=5*1000/1024
 if(x==0){Serial.print("I1=");Serial.print(value1);Serial.println(" mA, ");}//print 
current value to serial monitor
 if(value1>=100 && digitalRead(11)==HIGH ){digitalWrite(13,HIGH);x=1;} //if 
current >100ma & arduino2 has fault, activate the relays
 if(digitalRead(12)==HIGH){digitalWrite(13,LOW);x=0;} //if reset button os pressed, 
close the relays
 delay(1);
}
```

### Controlled MCU code :
```
float value1;
void setup() {
 // put your setup code here, to run once:
//pinMode(13,OUTPUT); //pin 13 as output(relay)
//pinMode(12,INPUT); //pin 12 as input (reset)
pinMode(11,OUTPUT); //pin 11 as OUTPUT (arduino2 fault)
digitalWrite(11,LOW); //pin 11 is low
Serial.begin(9600); //start serial
}
void loop() {
 // put your main code here, to run repeatedly:
 value1=(analogRead(A0)*1.8180048077); //voltage across shunt resistor or current 
since R=1 // 4882.8125=5*1000/1024
 Serial.print("I1=");Serial.print(value1);Serial.println(" mA, "); //print current 
value to serial monitor
 if(value1>=25){digitalWrite(11,HIGH);delay(500);} //if current >100ma, activate 
fault relay to notify arduino1
 else{digitalWrite(11,LOW);} //if no fault,close fault relay
 //if(digitalRead(12)==HIGH){digitalWrite(13,LOW);} //if reset button os pressed, 
close the relays
 delay(1);
}

```

## Oscilloscope results

Here is the current normal operation:

![normal voltage](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/Prototype/voltage%20normal.png)

Here is the voltage at normal operation:

![normal current](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/Prototype/current%20normal.png)

Here is the fault current:

![](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/Prototype/fault%20cuurent.png)

Here is the fault voltage:

![](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/Prototype/fault%20voltage.png)

Here is the voltage after clearing fault:

![](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/Prototype/cleared%20voltage.png)

Here is the current after clearing fault:

![](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/Prototype/cleared%20current.png)

And Here is the project prototype:

![](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/blob/main/assets/Prototype/IMG-20210712-WA0000.jpg)
