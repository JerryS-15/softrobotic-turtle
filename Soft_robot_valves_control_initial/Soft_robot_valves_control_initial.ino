//Set the valve numbers
const int Valve_1 = 7;    // H schaltet Valve_1
const int Valve_2 = 6;    // H schaltet Valve_2
const int Valve_3 = 5;    // H schaltet Valve_3
const int Valve_4 = 4;    // H schaltet Valve_4
const int Valve_5 = 3;    // H schaltet Valve_5
const int Valve_6 = 2;    // H schaltet Valve_6
const int Manuell = 8;    // H schaltet manuelle Kontrolle über Taster aus

// The state array of valves
bool valveStates[] = {false, false, false, false, false, false}; // The initial state is off

// Control the state manually
bool manualControlEnabled = false;//The initial state is off

void setup() {
  // Pinmode for OUTPUTS
  pinMode(Valve_1, OUTPUT);
  pinMode(Valve_2, OUTPUT);
  pinMode(Valve_3, OUTPUT);
  pinMode(Valve_4, OUTPUT);
  pinMode(Valve_5, OUTPUT);
  pinMode(Valve_6, OUTPUT);
  pinMode(Manuell, OUTPUT); 

  // Initialize the serial communication
  Serial.begin(9600);
  Serial.println("System ready. Send commands to control valves.");
  Serial.println("Commands: Valve_X on/off, manual control on/off, set all Valves, clear all Valves");
}

void loop() {
  // Cheack serial input
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove extra spaces and line breaks
    processCommand(command);
  }

  // The logic of manual control
  if (manualControlEnabled) {
    if (digitalRead(Manuell) == LOW) { // Press the buttom
      setAllValves(HIGH); // Open all the valves
    } else {
      setAllValves(LOW); // Close all the valves
    }
  }
}

// Process the serial command
void processCommand(String command) {
  if (command.equalsIgnoreCase("manual control on")) {
    manualControlEnabled = true;
    Serial.println("Manual control enabled.");
  } 
  else if (command.equalsIgnoreCase("manual control off")) {
    manualControlEnabled = false;
    Serial.println("Manual control disabled.");
  } 
  else if (command.equalsIgnoreCase("set all Valves")) {
    setAllValves(HIGH); // Open all the valves
    Serial.println("All valves opened.");
  } 
  else if (command.equalsIgnoreCase("clear all Valves")) {
    setAllValves(LOW); // Close all the valves
    Serial.println("All valves closed.");
  } 
  else if (command.startsWith("Valve_")) {
    int valveNumber = command.charAt(6) - '0'; // Get the number of valve
    if (valveNumber >= 1 && valveNumber <= 6) {
      if (command.endsWith("on")) {
        setValve(valveNumber, HIGH); // Open the valve
        Serial.println("Valve_" + String(valveNumber) + " opened.");
      } 
      else if (command.endsWith("off")) {
        setValve(valveNumber, LOW); // Close the valve
        Serial.println("Valve_" + String(valveNumber) + " closed.");
      } 
      else {
        Serial.println("Invalid command.");
      }
    } 
    else {
      Serial.println("Invalid valve number.");
    }
  } 
  else {
    Serial.println("Unknown command.");
  }
}

// Set the state of the single valve
void setValve(int valveNumber, bool state) {
  switch (valveNumber) {
    case 1:
      digitalWrite(Valve_1, state);
      valveStates[0] = state;
      break;
    case 2:
      digitalWrite(Valve_2, state);
      valveStates[1] = state;
      break;
    case 3:
      digitalWrite(Valve_3, state);
      valveStates[2] = state;
      break;
    case 4:
      digitalWrite(Valve_4, state);
      valveStates[3] = state;
      break;
    case 5:
      digitalWrite(Valve_5, state);
      valveStates[4] = state;
      break;
    case 6:
      digitalWrite(Valve_6, state);
      valveStates[5] = state;
      break;
    default:
      break;
  }
}

// Set the state of all the valves
void setAllValves(bool state) {
  digitalWrite(Valve_1, state);
  digitalWrite(Valve_2, state);
  digitalWrite(Valve_3, state);
  digitalWrite(Valve_4, state);
  digitalWrite(Valve_5, state);
  digitalWrite(Valve_6, state);
  for (int i = 0; i < 6; i++) {
    valveStates[i] = state;
  }
}