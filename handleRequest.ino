#include <string>

void handleRequest(EthernetClient client) {
	Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    int count = 0;
    clearBuffer(htmlRequestBuffer, htmlRequestBufferSize);
    requestCharCounter = 0;
    while (client.connected()) {
      if (client.available()) {
        // Here is where the POST data is.  
        while(client.available())
        {          
          htmlRequestBuffer[count] = client.read();
          char c = htmlRequestBuffer[count];
          count++;
        }
        break;
      }
    }
    Serial.println();
    Serial.println("REQUEST: ");
    Serial.println(htmlRequestBuffer);
    Serial.println("=======");
    Serial.println("TYPE: ");
    String type = readUntilSpace(htmlRequestBuffer, requestCharCounter, htmlRequestBufferSize);
    Serial.println(type.c_str());
    Serial.println("=======");
    Serial.println("ENDPOINT: ");
    String endpoint = readUntilSpace(htmlRequestBuffer, requestCharCounter, htmlRequestBufferSize);
    Serial.println(endpoint.c_str());
    Serial.println("=======");
    Serial.println("PROTOCOL: ");
    String protocol = readUntilSpace(htmlRequestBuffer, requestCharCounter, htmlRequestBufferSize);
    Serial.println(protocol.c_str());
    Serial.println("=======");
    Serial.println("BODY: ");
    Serial.println(freeMemory());
    char* firstOpenBrace = strstr(htmlRequestBuffer, "{");
    Serial.println(freeMemory());
    // char* body = calloc(500, sizeof(char));
    // if (body) {
    //   Serial.println("body");
    //   Serial.write(body);
    //   Serial.println("\n!!!!!!!!!!!!!!");
    // } else {
    //   Serial.println("DID NOT ALLOCATE MEMORY FOR BODY");
    // }
    // Serial.println(freeMemory());
    // Serial.println("firstOpenBrace");
    // Serial.write(firstOpenBrace);
    // Serial.println("\n!!!!!!!!!!!!!!");
    // Serial.println("body - firstOpenBrace");
    // Serial.println(body - firstOpenBrace);
    // Serial.println(firstOpenBrace - body);
    // Serial.println("\n!!!!!!!!!!!!!!");
    // strncpy(body, firstOpenBrace, body - firstOpenBrace);
    // Serial.println("body");
    // Serial.println(body);
    // Serial.println("freeMemory");
    // Serial.println(freeMemory());
    // free(body);
    // Serial.println("freed!!!");
    // Serial.println(freeMemory());
    // Serial.println("=======");

	// parse and handle request based on endpoint
    if (endpoint.startsWith("/config")) {
      // handleConfig(body, client);
    } else if (endpoint.startsWith("/device")) {  
      handleDevice(endpoint, client);
    } else if (endpoint.startsWith("/data")) {
      handleData(endpoint, client);
    } else if (endpoint.startsWith("/test")) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close"); 
      client.println();
      File root = SD.open("/");
      printDirectoryToClient(root, 0, client);
    } else if (endpoint.startsWith("/time")) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close"); 
      client.println();
      DateTime now = rtc.now();
      char formattedSDString[50];
      char* timeFormat = "MM/DD/YYYY hh:mm:ss";
      char* formattedTime = now.toString(timeFormat);
      strcpy(formattedSDString, formattedTime);
      char buffer[100];
      snprintf(buffer, 100, "%d", tankid);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      memset(buffer, 0, sizeof(Buffer));
      snprintf(buffer, 100, "%f", temp);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      memset(buffer, 0, sizeof(Buffer));
      snprintf(buffer, 100, "%f", tempset);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      memset(buffer, 0, sizeof(Buffer));
      snprintf(buffer, 100, "%f", pH);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      memset(buffer, 0, sizeof(Buffer));
      snprintf(buffer, 100, "%f", phset);
      strcat(formattedSDString, ",");
      strcat(formattedSDString, buffer);
      memset(buffer, 0, sizeof(Buffer));
      client.println(formattedSDString);
    } else {
      handleMisc(client);
    }
}