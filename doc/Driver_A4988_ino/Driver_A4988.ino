/*
 Name:		Driver_A4988.ino
 Created:	23/05/2019 09:34:30
 Author:	Denis
*/

// Define User Types below here or use a .h file
//

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62


#define LED_PIN            13

int  customDelayMapped; // Defines variables


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//
// Function for reading the Potentiometer
int speedUp() {
	int customDelay = analogRead(3); // Reads the potentiometer
	int newCustom = map(customDelay, 0, 1023, 10, 3000); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
	return newCustom;
}

// The setup() function runs once each time the micro-controller starts
void setup()
{
	pinMode(X_STEP_PIN, OUTPUT);
	pinMode(X_DIR_PIN, OUTPUT);
	pinMode(X_ENABLE_PIN, OUTPUT);

	pinMode(Y_STEP_PIN, OUTPUT);
	pinMode(Y_DIR_PIN, OUTPUT);
	pinMode(Y_ENABLE_PIN, OUTPUT);

	pinMode(Z_STEP_PIN, OUTPUT);
	pinMode(Z_DIR_PIN, OUTPUT);
	pinMode(Z_ENABLE_PIN, OUTPUT);


	digitalWrite(X_ENABLE_PIN, LOW);
	digitalWrite(X_DIR_PIN, HIGH);

	digitalWrite(Y_ENABLE_PIN, LOW);
	digitalWrite(Y_DIR_PIN, HIGH);

	digitalWrite(Z_ENABLE_PIN, LOW);
	digitalWrite(Z_DIR_PIN, HIGH);

	pinMode(3, INPUT);

}

// Add the main program code into the continuous loop() function
void loop()
{
	customDelayMapped = speedUp();

	digitalWrite(X_STEP_PIN, HIGH);
	digitalWrite(Y_STEP_PIN, HIGH);
	digitalWrite(Z_STEP_PIN, HIGH);
	//while (true); //à décommenter pour le test statique
	delayMicroseconds(customDelayMapped);
	digitalWrite(X_STEP_PIN, LOW);
	digitalWrite(Y_STEP_PIN, LOW);
	digitalWrite(Z_STEP_PIN, LOW);
	delayMicroseconds(customDelayMapped);

}
