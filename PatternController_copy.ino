#include <SPI.h>
#include <Ethernet.h>

#define PATTERN 4 // digital output for controlling pattern
#define IR_LEDS 5 // digital output for controlling IR_LEDS

// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

IPAddress ip(10,11,11,74); //static ip for arduino
// server defaults to port 5000
EthernetServer server(5000);


String readString;                                    
boolean gotAMessage = false; // whether or not you got a message from the client yet
bool isFixed = false;

void setup() {

  Serial.begin(9600);                     
  while (!Serial) {
    ;
  }
  
  pinMode(PATTERN, OUTPUT);
  pinMode(IR_LEDS, OUTPUT);

  digitalWrite(PATTERN, LOW);
  digitalWrite(IR_LEDS, LOW);

  Ethernet.begin(mac,ip);                            
  server.begin();
  Serial.print("Dia chi server la: ");
  Serial.println(Ethernet.localIP());
  Serial.println("Da ket noi thanh cong mang ethernet...");
}

void loop() {
	
	if(isFixed){
	  digitalWrite(PATTERN, LOW);
	  digitalWrite(IR_LEDS, HIGH);
	  delay(500);
	  digitalWrite(PATTERN, HIGH);
	  digitalWrite(IR_LEDS, LOW);
	  delay(500);
	  /*digitalWrite(PATTERN, LOW);
	  digitalWrite(IR_LEDS, LOW);
	  delay(497);
	  
	  digitalWrite(PATTERN, LOW);
	  digitalWrite(IR_LEDS, LOW);
	  delay(497);*/
	}else{

  EthernetClient client = server.available();           
  //delay(2000);

  if (client == true) { //if client:
    while (client.connected()) { 
      if (client.available()) {
        char c = client.read();               
        
        if (readString.length() < 100) {     
          
          readString += c;
        }
        
        Serial.println(readString);
        
        if (c=='1') {
          digitalWrite(PATTERN,HIGH);
          digitalWrite(IR_LEDS,LOW);
          client.println('1');
        }
        else if (c=='2'){
          digitalWrite(PATTERN,LOW);
          digitalWrite(IR_LEDS, HIGH);
          client.println('2');
        }
        else if (c=='3'){
          digitalWrite(PATTERN,LOW);
          digitalWrite(IR_LEDS, LOW);
          
          client.println('3');
        }
        else if (c=='4')
        {
          digitalWrite(PATTERN,LOW);
          digitalWrite(IR_LEDS, LOW);
          isFixed = true;
          client.println('4');
        }
        else if (c=='5')
        {
          digitalWrite(PATTERN,LOW);
          digitalWrite(IR_LEDS, LOW);
          isFixed = false;
          client.println('5');
        }
        
      } 
    } 
  }  
  else {
    digitalWrite(PATTERN,LOW);
    digitalWrite(IR_LEDS, LOW);
  }
	}

}

