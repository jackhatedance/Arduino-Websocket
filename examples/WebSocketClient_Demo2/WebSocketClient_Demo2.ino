#include <SPI.h>

#include <Ethernet.h>

// Here we define a maximum framelength to 64 bytes. Default is 256.
#define MAX_FRAME_LENGTH 64

// Define how many callback functions you have. Default is 1.
#define CALLBACK_FUNCTIONS 1

#include <WebSocketClient.h>

byte mac[]    = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x04 };
char* serverName = "echo.websocket.org";

EthernetClient client;
WebSocketClient webSocketClient;

void setup() {
  

  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();

  // Connect to the websocket server
  if (client.connect("echo.websocket.org", 80)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    while(1) {
      // Hang on failure
    }
  }

  // Handshake with the server
  webSocketClient.path = "/";
  webSocketClient.host = "echo.websocket.org";
  
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while(1) {
      // Hang on failure
    }  
  }
}

void loop() {
  int dataLength =100;
  char* data = new char[dataLength];
  
  
  if (client.connected()) {
    
    bool result = webSocketClient.getData(data, dataLength);

    if (result) {
      Serial.print("Received data: ");
      Serial.println(data);
    }else
    {
      Serial.print("Received data: NONE ");
    }
    
    // capture the value of analog 1, send it along
    pinMode(1, INPUT);
    String sendData = String(analogRead(1));
    
    Serial.print("Send data: ");
    Serial.println(sendData);
      
    webSocketClient.sendData(sendData);
    
  } else {
    
    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }
  
  // wait to fully let the client disconnect
  delay(3000);
}
