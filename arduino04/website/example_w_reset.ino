#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>  // Include the SPIFFS library for file system support
#include <Adafruit_NeoPixel.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

ESP8266WebServer server(80);

// Define NeoPixel matrix setup
#define PIN D1
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define NUMPIXELS MATRIX_WIDTH * MATRIX_HEIGHT

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize the matrix
  matrix.begin();
  matrix.show(); // Initialize all pixels to 'off'

  // Start SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  // Serve files from SPIFFS
  server.on("/", handleFileRead("/index.html"));
  server.on("/styles.css", handleFileRead("/styles.css"));
  server.on("/index.js", handleFileRead("/index.js"));

  // Handle POST request for setting color
  server.on("/setColor", HTTP_POST, handleSetColor);

  // Handle POST request for resetting the matrix
  server.on("/resetMatrix", HTTP_POST, []() {
    resetMatrix();
    server.send(200, "text/plain", "Matrix Reset");
  });

  // Catch-all handler for file not found
  server.onNotFound([]() {
    server.send(404, "text/plain", "File Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

// Function to reset the LED matrix
void resetMatrix() {
  for (int i = 0; i < NUMPIXELS; i++) {
    matrix.setPixelColor(i, matrix.Color(0, 0, 0));  // Turn off all LEDs
  }
  matrix.show();  // Refresh the matrix
}

// Handle POST request to set the LED matrix color
void handleSetColor() {
  if (server.hasArg("color") && server.hasArg("button")) {
    String color = server.arg("color");
    int button = server.arg("button").toInt();

    uint32_t rgbColor = convertToColor(color);  // Convert the color string to an RGB value
    matrix.setPixelColor(button, rgbColor);  // Update the matrix LED
    matrix.show();  // Refresh the matrix

    server.send(200, "text/plain", "Color Set");
  } else {
    server.send(400, "text/plain", "Invalid Request");
  }
}

// Helper function to read files from SPIFFS
bool handleFileRead(String path) {
  Serial.println("Reading file: " + path);
  if (path.endsWith("/")) {
    path += "index.html";
  }
  String contentType = getContentType(path);
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

// Helper function to get the MIME type of files
String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  if (filename.endsWith(".css")) return "text/css";
  if (filename.endsWith(".js")) return "application/javascript";
  return "text/plain";
}

// Helper function to convert a color string to an RGB value
uint32_t convertToColor(String color) {
  if (color == "red") return matrix.Color(255, 0, 0);
  if (color == "yellow") return matrix.Color(255, 255, 0);
  if (color == "green") return matrix.Color(0, 255, 0);
  if (color == "blue") return matrix.Color(0, 0, 255);
  return matrix.Color(0, 0, 0);  // Default to black if the color is not recognized
}
