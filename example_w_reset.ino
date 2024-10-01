#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>

// Replace with your Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

ESP8266WebServer server(80);

// Define NeoPixel matrix setup
#define PIN D1  // GPIO pin connected to the NeoPixel data input
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define NUMPIXELS MATRIX_WIDTH * MATRIX_HEIGHT

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected, IP: ");
  Serial.println(WiFi.localIP());

  // Initialize the matrix
  matrix.begin();
  matrix.show(); // Initialize all pixels to 'off'

  // Serve files (HTML, CSS, JS)
  server.on("/", handleRoot);
  server.on("/styles.css", handleCss);
  server.on("/index.js", handleJs);

  // Handle color change requests
  server.on("/setColor", HTTP_POST, handleSetColor);

  // Handle matrix reset (turn off all LEDs)
  server.on("/resetMatrix", HTTP_POST, []() {
    resetMatrix(); // Call the reset function
    server.send(200, "text/plain", "Matrix Reset");
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "File Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

// Function to reset the LED matrix (turn off all LEDs)
void resetMatrix() {
  for (int i = 0; i < NUMPIXELS; i++) {
    matrix.setPixelColor(i, matrix.Color(0, 0, 0)); // Set all LEDs to black (off)
  }
  matrix.show(); // Refresh the matrix to apply changes
}

// Handle POST request to set color
void handleSetColor() {
  if (server.hasArg("color") && server.hasArg("button")) {
    String color = server.arg("color");
    int button = server.arg("button").toInt();

    uint32_t rgbColor = convertToColor(color); // Convert color string to RGB
    matrix.setPixelColor(button, rgbColor); // Update the matrix LED
    matrix.show(); // Refresh the matrix

    server.send(200, "text/plain", "Color Set");
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

// Helper function to convert color names to RGB values
uint32_t convertToColor(String color) {
  if (color == "red") return matrix.Color(255, 0, 0);
  if (color == "yellow") return matrix.Color(255, 255, 0);
  if (color == "green") return matrix.Color(0, 255, 0);
  if (color == "blue") return matrix.Color(0, 0, 255);
  return matrix.Color(0, 0, 0); // Default to black if color is unrecognized
}
