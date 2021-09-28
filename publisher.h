// The Publisher class wraps the ethernet and MQTT libraries and handles the connection minutia
// so the calling code only needs to worry about pushing messages. The MQTT library can also
// listen to incoming messages but that has not been implemented here. The Publisher is pub only.
// By default it publishes to the mosquitto.org test broker.

#include <Ethernet.h>
#include <PubSubClient.h> // by Nick O'Leary, install via "Manage Libraries"

class Publisher {

    EthernetClient ethClient;
    PubSubClient mqttClient;

  public:

    // Default values can be changed via the instance
    byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // see sticker on Arduino Ethernet
    String server = "test.mosquitto.org";
    int port = 1883;
    String clientId = "arduino";

    bool connect() {
      if (!Ethernet.begin(mac)) {
        error("No IP address received via DHCP");
        return false;
      }
      delay(3000); // needs time to start

      mqttClient.setClient(ethClient);

      int sLen = server.length() + 1;
      char sBuf[sLen];
      server.toCharArray(sBuf, sLen);
      mqttClient.setServer(sBuf, port);

      int cLen = clientId.length() + 1;
      char cBuf[cLen];
      server.toCharArray(cBuf, cLen);
      if (!mqttClient.connect(cBuf)) {
        error("Not connected to MQTT broker");
        return false;
      }

      return true;
    }

    bool publish(String topic, String message) {
      int tLen = topic.length() + 1;
      char tBuf[tLen];
      topic.toCharArray(tBuf, tLen);

      int mLen = message.length() + 1;
      char mBuf[mLen];
      message.toCharArray(mBuf, mLen);

      return mqttClient.publish(tBuf, mBuf);
    }

  private:

    void error(String message) {
      Serial.print("! ");
      Serial.println(message);
    }

};
