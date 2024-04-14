#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "Kenny"
#define STAPSK "coal1080"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);
const int led =10;


const int redled = 15;
const int yellowled = 13;
const int greenled = 12;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n<head>\r\n  <meta charset=\"UTF-8\">\r\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n  <title>Traffic Light Control</title>\r\n  <style>\r\n    body {\r\n      display: flex;\r\n      justify-content: center;\r\n      align-items: center;\r\n      height: 100vh;\r\n      margin: 0;\r\n      background-color: #f0f0f0;\r\n    }\r\n\r\n    .traffic-signal {\r\n      width: 100px;\r\n      height: 300px;\r\n      background-color: #333;\r\n      border-radius: 10px;\r\n      display: flex;\r\n      flex-direction: column;\r\n      justify-content: space-between;\r\n      align-items: center;\r\n    }\r\n\r\n    .light {\r\n      width: 60px;\r\n      height: 60px;\r\n      border-radius: 50%;\r\n      margin: 10px;\r\n      transition: background-color 0.9s;\r\n      cursor: pointer;\r\n    }\r\n\r\n    .red {\r\n      background-color: red;\r\n    }\r\n\r\n    .yellow {\r\n      background-color: yellow;\r\n    }\r\n\r\n    .green {\r\n      background-color: green;\r\n    }\r\n\r\n    .blinking {\r\n      animation: blink 0.5s infinite alternate;\r\n    }\r\n\r\n    @keyframes blink {\r\n      from {\r\n        opacity: 1;\r\n      }\r\n      to {\r\n        opacity: 0;\r\n      }\r\n    }\r\n  </style>\r\n</head>\r\n<body>\r\n  <div class=\"traffic-signal\">\r\n    <div class=\"light red\" onclick=\"changeLight(\'red\')\"></div>\r\n    <div class=\"light yellow\" onclick=\"changeLight(\'yellow\')\"></div>\r\n    <div class=\"light green\" onclick=\"changeLight(\'green\')\"></div>\r\n  </div>\r\n\r\n  <script>\r\n    let isRedOn = false;\r\n    let isGreenOn = false;\r\n    let blinkingInterval = null;\r\n\r\n    function changeLight(clickedLight) {\r\n      var lights = document.querySelectorAll(\'.light\');\r\n\r\n      // Turn off all lights\r\n      lights.forEach(function(light) {\r\n        light.classList.remove(\'red\', \'yellow\', \'green\', \'blinking\');\r\n      });\r\n\r\n      // Stop blinking yellow light if it was previously blinking\r\n      if (blinkingInterval) {\r\n        clearInterval(blinkingInterval);\r\n      }\r\n\r\n      // Handle red light click\r\n      if (clickedLight === \'red\') {\r\n        if (isGreenOn) {\r\n          lights[1].classList.add(\'yellow\'); // Yellow turns on\r\n          setTimeout(function() {\r\n            lights[1].classList.remove(\'yellow\'); // Yellow turns off\r\n            lights[0].classList.add(\'red\'); // Red turns on\r\n          }, 500);\r\n          isRedOn = true;\r\n          isGreenOn = false;\r\n        } else {\r\n          lights[0].classList.add(\'red\'); // Red turns on\r\n          isRedOn = true;\r\n        }\r\n      }\r\n\r\n      // Handle green light click\r\n      else if (clickedLight === \'green\') {\r\n        if (isRedOn) {\r\n          lights[1].classList.add(\'yellow\'); // Yellow turns on\r\n          setTimeout(function() {\r\n            lights[1].classList.remove(\'yellow\'); // Yellow turns off\r\n            lights[2].classList.add(\'green\'); // Green turns on\r\n          }, 500);\r\n          isRedOn = false;\r\n          isGreenOn = true;\r\n        } else {\r\n          lights[2].classList.add(\'green\'); // Green turns on\r\n          isGreenOn = true;\r\n        }\r\n      }\r\n\r\n      // Handle yellow light click\r\n      else if (clickedLight === \'yellow\') {\r\n        lights[1].classList.add(\'yellow\'); // Yellow turns on\r\n        blinkingInterval = setInterval(function() {\r\n          lights[1].classList.toggle(\'blinking\');\r\n        }, 500); // Blink every 0.5 seconds\r\n      }\r\n    }\r\n  </script>\r\n</body>\r\n</html>");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.on("/redon", []() 
  {server.send(200, "text/plain", "Red LED is on");
  digitalWrite(15, HIGH); 
  });
    
    server.on("/yellowon", []() 
  {server.send(200, "text/plain", "Yellow LED is on");
  digitalWrite(13, HIGH); 
  });

  server.on("/greenon", []() 
  {server.send(200, "text/plain", "Green LED is on");
  digitalWrite(12, HIGH); 
  });

  server.on("/redoff", []() {
    server.send(200, "text/plain", "Red LED is off");
  digitalWrite(15, LOW); });

  server.on("/yellowoff", []() {
    server.send(200, "text/plain", "Yellow LED is off");
  digitalWrite(13, LOW); 
  });
  server.on("/greenoff", 
  []() {
    server.send(200, "text/plain", "Red LED is off");
    digitalWrite(12, LOW); });







  server.onNotFound(handleNotFound);


  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
