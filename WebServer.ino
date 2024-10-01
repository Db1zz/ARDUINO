/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <StreamString.h>

#ifndef STASSID
#define STASSID "42Wolfsburg_FabLab"
#define STAPSK "0nly5ky15theL1m17"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);
const int led = 13;

// Handle root request
void handleRoot() {
  digitalWrite(led, 1);

  String temp;

  temp.concat("<!DOCTYPE html>\n");
  temp.concat("<html lang='en'>\n");
  temp.concat("<head>\n");
  temp.concat("    <meta charset='UTF-8'>\n");
  temp.concat("    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n");
  temp.concat("    <title>8x8 Fixed Grid</title>\n");
  temp.concat("    <link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css' rel='stylesheet'>\n");
  temp.concat("    <link rel='stylesheet' href='styles.css'>\n");
  temp.concat("</head>\n");
  temp.concat("<body>\n");
  temp.concat("    <div class='container d-flex justify-content-center align-items-center min-vh-100'>\n");
  temp.concat("        <div class='box row'>\n");
  temp.concat("            <div class='col-2 d-flex flex-column justify-content-center align-items-center'>\n");
  temp.concat("                <button class='btn color-btn RED mb-2' data-color='red'>Red</button>\n");
  temp.concat("                <button class='btn color-btn YELLOW mb-2' data-color='yellow'>Yellow</button>\n");
  temp.concat("                <button class='btn color-btn GREEN mb-2' data-color='green'>Green</button>\n");
  temp.concat("                <button class='btn color-btn BLUE' data-color='blue'>Blue</button>\n");
  temp.concat("            </div>\n");

  temp.concat("            <div class='col-8 d-flex justify-content-center align-items-center'>\n");
  temp.concat("                <div class='grid-container'>\n");
  temp.concat("                    <div class='grid'>\n");
  temp.concat("                        <button class='grid-btn'></button>\n"); // Example button
  temp.concat("                        <!-- Repeat buttons for 8x8 grid -->\n");
  temp.concat("                    </div>\n");
  temp.concat("                </div>\n");
  temp.concat("            </div>\n");

  temp.concat("            <div class='col-2 d-flex flex-column justify-content-center align-items-center'>\n");
  temp.concat("                <button class='square-btn mb-2'></button>\n");
  temp.concat("                <button class='square-btn mb-2'></button>\n");
  temp.concat("                <button class='square-btn'></button>\n");
  temp.concat("            </div>\n");
  temp.concat("        </div>\n");
  temp.concat("    </div>\n");
  temp.concat("    <script src='index.js'></script>\n");
  temp.concat("</body>\n");
  temp.concat("</html>\n");

  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

// Handle JavaScript file serving
void handleJS() {
  String jsCode;

  jsCode.concat("// JavaScript Code to handle the color selection and apply to grid buttons\n\n");
  jsCode.concat("let selectedColor = '';\n");
  jsCode.concat("const colorButtons = document.querySelectorAll('.color-btn');\n");
  jsCode.concat("const gridButtons = document.querySelectorAll('.grid-btn');\n");

  jsCode.concat("colorButtons.forEach(button => {\n");
  jsCode.concat("    button.addEventListener('click', function() {\n");
  jsCode.concat("        selectedColor = this.getAttribute('data-color');\n");
  jsCode.concat("        colorButtons.forEach(btn => btn.classList.remove('active-color'));\n");
  jsCode.concat("        this.classList.add('active-color');\n");
  jsCode.concat("    });\n");
  jsCode.concat("});\n");

  jsCode.concat("gridButtons.forEach(button => {\n");
  jsCode.concat("    button.addEventListener('click', function() {\n");
  jsCode.concat("        if (selectedColor) {\n");
  jsCode.concat("            this.style.backgroundColor = selectedColor;\n");
  jsCode.concat("        } else {\n");
  jsCode.concat("            alert('Please select a color first.');\n");
  jsCode.concat("        }\n");
  jsCode.concat("    });\n");
  jsCode.concat("});\n");

  server.send(200, "application/javascript", jsCode);
}

// Handle CSS file serving
void handleCSS() {
  String cssCode;

  cssCode.concat("/* Background Color for the Page */\n");
  cssCode.concat("body {\n");
  cssCode.concat("    background-color: #202040;\n");
  cssCode.concat("    margin: 0;\n");
  cssCode.concat("    padding: 0;\n");
  cssCode.concat("}\n");

  cssCode.concat(".box {\n");
  cssCode.concat("    background-color: #543864;\n");
  cssCode.concat("    width: 80%;\n");
  cssCode.concat("    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);\n");
  cssCode.concat("    padding: 20px;\n");
  cssCode.concat("}\n");

  cssCode.concat(".grid-container {\n");
  cssCode.concat("    display: flex;\n");
  cssCode.concat("    justify-content: center;\n");
  cssCode.concat("}\n");

  cssCode.concat(".grid {\n");
  cssCode.concat("    display: grid;\n");
  cssCode.concat("    grid-template-columns: repeat(8, 50px);\n");
  cssCode.concat("    grid-template-rows: repeat(8, 50px);\n");
  cssCode.concat("    gap: 5px;\n");
  cssCode.concat("}\n");

  server.send(200, "text/css", cssCode);
}

// Handle not found (404)
void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: " + server.uri() + "\n";
  
  // Convert the method to a String explicitly
  message += "Method: " + String((server.method() == HTTP_GET) ? "GET" : "POST") + "\n";
  
  // Convert the number of arguments to a String
  message += "Arguments: " + String(server.args()) + "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}


// Simple graph drawing in SVG format
void drawGraph() {
  String out;
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"100\" height=\"100\">\n";
  out += "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
  out += "<circle cx=\"50\" cy=\"50\" r=\"40\" stroke=\"black\" stroke-width=\"2\" fill=\"yellow\"/>\n";
  out += "<line x1=\"10\" y1=\"10\" x2=\"90\" y2=\"90\" stroke=\"black\" stroke-width=\"2\"/>\n";
  out += "</svg>\n";
  server.send(200, "image/svg+xml", out);
}

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/index.js", handleJS);
  server.on("/styles.css", handleCSS);
  server.on("/graph", drawGraph);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
