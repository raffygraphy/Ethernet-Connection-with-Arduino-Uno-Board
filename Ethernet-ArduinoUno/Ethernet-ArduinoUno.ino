#include <SPI.h>
#include <Ethernet.h>

int redLed = 1;
int greenLed = 2;
int blueLed = 3;

// 6 bytes MAC address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xEF, 0xED };
EthernetServer server(80);
String readString;

void setup() {
  Serial.begin(9600);
  // pinMode initialize
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  Ethernet.begin(mac);
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();

  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);

        if (readString.length() < 100) {
          readString += c;
        }

        if (c == '\n') {
          Serial.print(readString);
          handleRequest(readString);
          delay(1);
          client.stop();
          readString = "";
        }
      }
    }
  }
}

void handleRequest(String request) {
  if (request.indexOf("?buttonRedOn") > 0) {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, LOW);
  }

  if (request.indexOf("?buttonRedOff") > 0) {
    digitalWrite(redLed, LOW);
  }

  if (request.indexOf("?buttonGreenOn") > 0) {
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(blueLed, LOW);
  }

  if (request.indexOf("?buttonGreenOff") > 0) {
    digitalWrite(greenLed, LOW);
  }

  if (request.indexOf("?buttonBlueOn") > 0) {
    digitalWrite(blueLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
  }

  if (request.indexOf("?buttonBlueOff") > 0) {
    digitalWrite(blueLed, LOW);
  }

  sendHtmlResponse();
}

void sendHtmlResponse() {
  EthernetClient client = server.available();

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>RGB LED Control</title>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1>RGB LED Control</h1>");

  // Red LED buttons
  client.println("<h2>Red LED</h2>");
  client.println("<a href=\"/?buttonRedOn\"><button>ON</button></a>");
  client.println("<a href=\"/?buttonRedOff\"><button>OFF</button></a>");

  // Green LED buttons
  client.println("<h2>Green LED</h2>");
  client.println("<a href=\"/?buttonGreenOn\"><button>ON</button></a>");
  client.println("<a href=\"/?buttonGreenOff\"><button>OFF</button></a>");

  // Blue LED buttons
  client.println("<h2>Blue LED</h2>");
  client.println("<a href=\"/?buttonBlueOn\"><button>ON</button></a>");
  client.println("<a href=\"/?buttonBlueOff\"><button>OFF</button></a>");

  client.println("</body>");
  client.println("</html>");

  delay(1);
  client.stop();
}
