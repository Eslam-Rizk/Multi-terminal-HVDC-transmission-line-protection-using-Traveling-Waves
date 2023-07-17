# Multi terminal HVDC transmission line protection using Traveling Waves
This project utilizes wavelet analysis to provide accurate fault locations for multi-terminal HVDC transmission line after isolating fault transmission line from the grid by two relays at the two ends of the transmission line.This technique can provide fast fault isolation, accurate fault location and system continuity.
This project has two sides: 
1-MATLAB simulation.
2-Emedded systems prototype.
# MATLAB simulink

Check full project on Matlab [here](https://github.com/Eslam-Rizk/Multi-terminal-HVDC-transmission-line-protection-using-Traveling-Waves/tree/main/HVDC%20Matlab)

Here is the simulink code:
```octave
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

* Atmel Stuio
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
```c
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE F_CPU/16/BAUD-1

void init_uart() {
    // Set baud rate
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    
    // Enable receiver and transmitter
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send_byte(uint8_t data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    
    // Put data into buffer, sends the data
    UDR0 = data;
}

void init_adc() {
    // Set reference voltage to AVcc
    ADMUX |= (1 << REFS0);
    
    // Set prescaler to 128
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Enable ADC
    ADCSRA |= (1 << ADEN);
}

uint16_t read_adc(uint8_t channel) {
    // Select ADC channel
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    // Start single conversion
    ADCSRA |= (1 << ADSC);
    
    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));
    
    // Return ADC value
    return ADC;
}

int main() {
    // Set pin 11 as output (arduino2 fault)
    DDRB |= (1 << DDB3);
    // Set pin 11 to low
    PORTB &= ~(1 << PORTB3);
	
	// Initialize UART
    init_uart();

	// Initialize ADC
    init_adc();
    
    // Enable global interrupts
    sei();
	

	while(1) {
		// Read the voltage across the shunt resistor or the current
		uint16_t adc_value = read_adc(0);
		// If current is greater than or equal to 100mA and arduino2 has fault, activate the relay
		if (adc_value >= 100) {
			PORTB |= (1 << PORTB3);
			}
		else{PORTB &= ~(1 << PORTB3);}
		// Print the current value to the serial monitor
		char buffer[10];
		sprintf(buffer, "%u", adc_value);
		for (int i = 0; buffer[i] != '\0'; i++) {
			uart_send_byte(buffer[i]);
		}
		uart_send_byte('\r');
		uart_send_byte('\n');
		
		// Wait for a short duration
		_delay_ms(10);
	}
    
	return 0;
}
```

### Controlled MCU code :
```c
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE F_CPU/16/BAUD-1

void init_uart() {
    // Set baud rate
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    
    // Enable receiver and transmitter
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send_byte(uint8_t data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    
    // Put data into buffer, sends the data
    UDR0 = data;
}

void init_adc() {
    // Set reference voltage to AVcc
    ADMUX |= (1 << REFS0);
    
    // Set prescaler to 128
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Enable ADC
    ADCSRA |= (1 << ADEN);
}

uint16_t read_adc(uint8_t channel) {
    // Select ADC channel
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    // Start single conversion
    ADCSRA |= (1 << ADSC);
    
    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));
    
    // Return ADC value
    return ADC;
}

int main() {
    // Set pin 13 as output (relay)
    DDRB |= (1 << DDB5);
    // Set pin 12 and 11 as input (reset and arduino2 fault)
    DDRB &= ~((1 << DDB4) | (1 << DDB3));
    // Set pin 13 to low
    PORTB &= ~(1 << PORTB5);
	
	// Initialize UART
    init_uart();

	// Initialize ADC
    init_adc();
    
    // Enable global interrupts
    sei();
	
	// Initialize relay state to off
	uint8_t relay_state = 0;

	while(1) {
		// Read the voltage across the shunt resistor or the current
		uint16_t adc_value = read_adc(0);
		// If current is greater than or equal to 100mA and arduino2 has fault, activate the relay
		if (adc_value >= 100 && (PINB & (1 << PINB3))) {
			PORTB |= (1 << PORTB5);
			relay_state = 1;
		}
		// If reset button is pressed, close the relay
		if (!(PINB & (1 << PINB4))) {
			PORTB &= ~(1 << PORTB5);
			relay_state = 0;
		}
		// Print the current value to the serial monitor
		if (relay_state == 0) {
			char buffer[10];
			sprintf(buffer, "%u", adc_value);
			for (int i = 0; buffer[i] != '\0'; i++) {
				uart_send_byte(buffer[i]);
			}
			uart_send_byte('\r');
			uart_send_byte('\n');
		}
		// Wait for a short duration
		_delay_ms(10);
	}
    
	return 0;
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
