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
bool manualControlEnabled = false; //The initial state is off

// Crawl state & timer
bool isCrawling = false;
unsigned long valve1StartTime = 0;
unsigned long valve2StartTime = 0;
unsigned long valve3StartTime = 0;
unsigned long valve4StartTime = 0;
unsigned long valve5StartTime = 0;
unsigned long valve6StartTime = 0;
bool valve1State = false; // left wing
bool valve2State = false; // right wing
bool valve3State = false;
bool valve4State = false; // Body + Bubbling
bool valve5State = false; // For tail Upper part
bool valve6State = false; // For tail Bottom part

bool isCrawlingLeft = false;
bool isCrawlingRight = false;

bool isCute = false;

// Swim state & timer
bool isSwimming = false;
unsigned long swimValve12StartTime = 0;
unsigned long swimValveInitStartTime = 0;
unsigned long swimValve3StartTime = 0;
unsigned long swimValve4StartTime = 0;
unsigned long swimValve5StartTime = 0;
unsigned long swimValve6StartTime = 0;
bool swimValve12State = false; // Actual state for valve 1,2
bool swimValveInitState = false; // Initial state for valve 1,2
bool swimValve3State = false;
bool swimValve4State = false;
bool swimValve5State = false;
bool swimValve6State = false;

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
  Serial.println("Turtle: crawl, swim, cl, cr, cute, sc, stop");
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

  if (isCrawling) {
    executeCrawl(300, 300);
  }

  if (isSwimming) {
    executeSwim(1000, 300);
  }

  if (isCrawlingLeft) {
    executeCrawlLeftTurning(300, 500);
  }

  if (isCrawlingRight) {
    executeCrawlRightTurning(300, 500);
  }

  if (isCute) {
    beCute(5000);
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
  else if (command.equalsIgnoreCase("crawl")) {
    isCrawling = true;
    isSwimming = false;
    isCrawlingLeft = false;
    isCrawlingRight = false;
    Serial.println("AI Turtle crawling starts.");
  }
  else if (command.equalsIgnoreCase("swim")) {
    isSwimming = true;
    isCrawling = false;
    isCrawlingLeft = false;
    isCrawlingRight = false;
    Serial.println("AI Turtle swimming starts.");
  }
  else if (command.equalsIgnoreCase("cl")) {
    isSwimming = false;
    isCrawling = false;
    isCrawlingLeft = true;
    isCrawlingRight = false;
    Serial.println("AI Turtle crawling turning left.");
  }
  else if (command.equalsIgnoreCase("cr")) {
    isSwimming = false;
    isCrawling = false;
    isCrawlingLeft = false;
    isCrawlingRight = true;
    Serial.println("AI Turtle crawling turning right.");
  }
  else if (command.equalsIgnoreCase("stop")) {
    isCrawling = false;
    isSwimming = false;
    isCrawlingLeft = false;
    isCrawlingRight = false;
    isCute = false;
    setAllValves(LOW);
    Serial.println("Execution stops.");
  }
  else if (command.equalsIgnoreCase("cute")) {
    isCute = true;
    Serial.println("AI Turtle is acting cute.");
  }
  else if (command.equalsIgnoreCase("sc")) {
    isCute = false;
    Serial.println("AI Turtle stop acting cute.");
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

// Execute crawling
// Using Valve 1, 2, 5, 6
void executeCrawl(int hands_dur, int tail_dur) {

  if (!valve1State && !valve2State) {
    setValve(1, HIGH);
    valve1State = true;
    valve1StartTime = millis();
    Serial.print("Hand 1 move for ");
    Serial.print(hands_dur);
    Serial.println(" ms");
  }
  else if (valve1State && millis() - valve1StartTime >= hands_dur) {
    setValve(1, LOW);
    valve1State = false;
    setValve(2, HIGH);
    valve2State = true;
    valve2StartTime = millis();
    Serial.print("Hand 2 move for ");
    Serial.print(hands_dur);
    Serial.println(" ms");
  }
  else if (valve2State && millis() - valve2StartTime >= hands_dur) {
    setValve(2, LOW);
    valve2State = false;
  }

  if (!valve5State && !valve6State) {
    // For tail flapping
    setValve(5, HIGH);
    
    // For always pumping 6
    // setValve(6, HIGH);

    // For tail flapping
    valve5State = true;
    valve5StartTime = millis();

    Serial.print("Tail (side 5) move for ");
    Serial.print(tail_dur);
    Serial.println(" ms");
  }
  else if (valve5State && millis() - valve5StartTime >= tail_dur) {
    // For tail flapping
    setValve(5, LOW);

    valve5State = false;
    setValve(6, HIGH);
    valve6State = true;
    valve6StartTime = millis();
    Serial.print("Tail (side 6) move for ");
    Serial.print(tail_dur);
    Serial.println(" ms");
  }
  else if (valve6State && millis() - valve6StartTime >= tail_dur) {
    setValve(6, LOW);
    valve6State = false;
  }

  // if (!valve4State) {
  //   setValve(4, HIGH);
  //   valve4State = true;
  //   valve4StartTime = millis();
  //   Serial.print("Body (valve 4) is on!");
  // }
  // else if (valve4State && millis() - valve4StartTime >= tail_dur) {
  //   setValve(4, HIGH);
  //   valve4State = true;
  //   valve4StartTime = millis();
  // }

}

// Execute swimming
// Using Valve 1, 2, 5, 6
void executeSwim(int hands_dur, int tail_dur) {

  // if (!swimValve12State && !swimValveInitState) {
  //   // setValve(1, HIGH);
  //   // setValve(2, HIGH);  // comment off if let valve 1,2 always on

  //   swimValveInitState = true;
  //   swimValveInitStartTime = millis();
  //   Serial.print("HANDS SWIM STARTING.");
  //   // Serial.print(hands_dur);
  //   // Serial.println(" ms");
  // }
  // else if (swimValveInitState && millis() - swimValveInitStartTime >= hands_dur) {
  //   setValve(1, HIGH);
  //   setValve(2, HIGH);
  //   swimValve12State = true;
  //   swimValveInitState = false;
  //   swimValve12StartTime = millis();
  //   Serial.print("Both hands move for ");
  //   Serial.print(hands_dur);
  //   Serial.println(" ms");
  // }
  // else if (swimValve12State && millis() - swimValve12StartTime >= hands_dur) {
  //   // setValve(1, HIGH);
  //   // setValve(2, HIGH);

  //   setValve(1, LOW);
  //   setValve(2, LOW);
  //   swimValve12State = false;
  //   Serial.print("Both hands rest for ");
  //   Serial.print(hands_dur);
  //   Serial.println(" ms");
  // }

  if (!swimValve5State && !swimValve6State) {
    setValve(5, HIGH);
    swimValve5State = true;
    swimValve5StartTime = millis();
    Serial.print("Tail (side 5) move for ");
    Serial.print(tail_dur);
    Serial.println(" ms");
  }
  else if (swimValve5State && millis() - swimValve5StartTime >= tail_dur) {
    setValve(5, LOW);
    swimValve5State = false;
    setValve(6, HIGH);
    swimValve6State = true;
    swimValve6StartTime = millis();
    Serial.print("Tail (side 6) move for ");
    Serial.print(tail_dur);
    Serial.println(" ms");
  }
  else if (swimValve6State && millis() - swimValve6StartTime >= tail_dur) {
    setValve(6, LOW);
    swimValve6State = false;
  }
}

// Execute crawling: left turning
// Using Valve 2, 6
void executeCrawlLeftTurning(int hands_dur, int tail_dur) {

  if (!valve1State && !valve2State) {
    // setValve(1, HIGH);
    valve1State = true;
    valve1StartTime = millis();
    Serial.print("Hand 1 move for ");
    Serial.print(hands_dur);
    Serial.println(" ms");
  }
  else if (valve1State && millis() - valve1StartTime >= hands_dur) {
    // setValve(1, LOW);
    valve1State = false;
    setValve(2, HIGH);
    valve2State = true;
    valve2StartTime = millis();
    Serial.print("Hand 2 move for ");
    Serial.print(hands_dur);
    Serial.println(" ms");
  }
  else if (valve2State && millis() - valve2StartTime >= hands_dur) {
    setValve(2, LOW);
    valve2State = false;
  }

  if (!valve5State && !valve6State) {
    // setValve(5, HIGH);
    
    setValve(6, HIGH);

    // valve5State = true;
    // valve5StartTime = millis();
    Serial.print("Tail (side 5) move for ");
    Serial.print(tail_dur);
    Serial.println(" ms");
  }
  else if (valve5State && millis() - valve5StartTime >= tail_dur) {
    // setValve(5, LOW);
    valve5State = false;
    setValve(6, HIGH);
    valve6State = true;
    valve6StartTime = millis();
    Serial.print("Tail (side 6) move for ");
    Serial.print(tail_dur);
    Serial.println(" ms");
  }
  else if (valve6State && millis() - valve6StartTime >= tail_dur) {
    setValve(6, LOW);
    valve6State = false;
  }

}

// Execute crawling: right turning
// Using Valve 1, 6
void executeCrawlRightTurning(int hands_dur, int tail_dur) {

  if (!valve1State && !valve2State) {
    // setValve(1, HIGH);
    valve2State = true;
    valve2StartTime = millis();
    Serial.print("Right turning START.");
  }
  else if (valve2State && millis() - valve2StartTime >= hands_dur) {
    // setValve(2, LOW);
    valve2State = false;
    setValve(1, HIGH);
    valve1State = true;
    valve1StartTime = millis();
    Serial.print("Hand 1 move for ");
    Serial.print(hands_dur);
    Serial.println(" ms");
  }
  else if (valve1State && millis() - valve1StartTime >= hands_dur) {
    setValve(1, LOW);
    valve1State = false;
  }

  if (!valve5State && !valve6State) {
    // setValve(5, HIGH);
    
    setValve(6, HIGH);

    // valve5State = true;
    // valve5StartTime = millis();
    Serial.println("Tail (side 6) pumping.");
  }
  else if (valve5State && millis() - valve5StartTime >= tail_dur) {
    // setValve(5, LOW);
    valve5State = false;
    setValve(6, HIGH);
    valve6State = true;
    valve6StartTime = millis();
    Serial.print("Tail (side 6) move for ");
    Serial.print(tail_dur);
    Serial.println(" ms");
  }
  else if (valve6State && millis() - valve6StartTime >= tail_dur) {
    setValve(6, LOW);
    valve6State = false;
  }

}

void beCute(int cuteTime) {

  setValve(4, HIGH);
  valve4State = true;
  valve4StartTime = millis();
  Serial.print("Body (valve 4) is on!");

  if (millis() - valve4StartTime >= cuteTime) {
    setValve(4, LOW);
    valve4State = false;
  }
}