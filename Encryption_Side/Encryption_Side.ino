#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define STASSID "ESP8266"
#define STAPSK  "ESP8266Test"

const char* ssid     = STASSID;
const char* password = STAPSK;


ESP8266WebServer server(80);

const String postForms = "<html><head><title>ESP8266 Web Server - text encryption</title><style>body{background-color:#bfeef3;color:#676767;text-align:center}input{width:250px;height:40px;padding:0px 10px;background:#fff;border:1px solid #cacaca;border-radius:5px;margin:10px;color:#676767}</style></head><body><h1>Enter your text to encrypt</h1><br><form method=\"post\" enctype=\"text/plain\" action=\"/encrypt/\"><input type=\"text\" placeholder=\"Text\" name=\"text\"><br><input type=\"submit\" value=\"Submit\"></form></body></html>";


char char_list_array[62] = {'Q','4','m','z','2','A','0','S','v','b','N','p','R','E','c','t','F','U','H','i','J','K','Y','6','X','0','8','r','d','V','3','x','y','a','1','B','C','Z','5','e','j','I','n','f','T','u','M','k','h','P','l','g','9','q','7','L','O','G','W','w','s','D'};

String encrypt_text(String text){  
  char message[text.length()+10];
  text.toCharArray(message,text.length()+1);  
  char ch;
  int i;
  for(i = 0; message[i] != '\0'; ++i){
    if(message[i]>='A' && message[i] <= 'Z')
    {
      message[i] = char_list_array[message[i]-65];
    }
    else if(message[i] >='0' && message[i] <= '9'){
      message[i] = char_list_array[message[i]+4];
    }
    else if (message[i]>='a' && message[i] <= 'z')
    {
      message[i] = char_list_array[message[i]-71];
    }
    else{
      message[i] = message[i];
    }
  }
  return String(message);
}

void handleRoot() {
  server.send(200, "text/html", postForms);
}

void handlePlain() {

  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  }
  else 
  {
    String message = server.arg("plain").substring(5);
    String postForms1 = "<html><head><title>ESP8266 Web Server - text encryption</title><style>body{background-color:#bfeef3;color:#676767;text-align:center}input{width:250px;height:40px;padding:0px 10px;background:#fff;border:1px solid #cacaca;border-radius:5px;margin:10px;color:#676767}</style></head><body><h1>Enter your text to encrypt</h1><br><form method=\"post\" enctype=\"text/plain\" action=\"\"><input type=\"text\" placeholder=\"Text\" name=\"text\"><br><input type=\"submit\" value=\"Submit\"></form><br><h3>The result of \""+ message+"\" encryption is \""+encrypt_text(message) +"\"</h3></body></html>";
    server.send(200, "text/html",postForms1);
  }
}


void handleNotFound() {
  String message = "<html><head><title>ESP8266 Web Server - text encryption</title><style>body{background-color:#bfeef3;color:#676767;text-align:center}input{width:250px;height:40px;padding:0px 10px;background:#fff;border:1px solid #cacaca;border-radius:5px;margin:10px;color:#676767}</style></head><body><h1>404 - File Not Found</h1></body></html>";

  server.send(404, "text/html", message);
}

void setup(void) {

  IPAddress ip(192,168,10,1);
  IPAddress gateway(192,168,10,2);
  IPAddress subnet(255,255,255,0);

  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/", handleRoot);
  server.on("/encrypt/", handlePlain);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop(void) {
  server.handleClient();
}
