#include <WiFi101.h>
#include <WiFiUdp.h>
#include "./src/computeDate.h"
#include "./src/printTestResults.h"
#include "./src/arrLabels.h"
#include "./src/modulateOutArray.h"

//#define TEST 1

bool isRunning = false;
int outArr[65];

int status = WL_IDLE_STATUS;

char homessid[] = "TashaNet";         // SSID
char homepass[] = "Mice4Cats";        // password
char workssid[] = "SM-G930P8BF";      // SSID
char workpass[] = "5088318030";       // password
// int keyIndex = 0;		          // your network key Index number (needed only for WEP)

unsigned int localPort = 2390; // local port to listen for UDP packets

IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

int count = 0;
int rcvCount = 0; // timing until packet recieved
int ii;

unsigned long secsLeft;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD)
    {
        Serial.println("WiFi shield not present");
        // don't continue:
        while (true)
        ;
    }

    // attempt to connect to WiFi network:
    while (status != WL_CONNECTED)
    {
        status = WiFi.begin(homessid, homepass);
        delay(5000);
        if (status != WL_CONNECTED) {
            status  = WiFi.begin(workssid, workpass);
            delay(5000);
        }
    }

    Serial.println("Connected to wifi");
    printWiFiStatus();

    Serial.println("\nStarting connection to server...");
    delay(1000); // Not sure that this helps before the UDP connection is sent
    Udp.begin(localPort);
}

void loop()
{
 
    const int REQUESTDELAY = 5000;               // 5 Seconds minimum between requests
    const unsigned long LOOPDELAY = 120000UL;    // 2 minutes between successful requests
    const int DELAYINCREMENT = 20;
    const int WAITLIMIT = 200;
    int delaycount = 0;

    sendNTPpacket(timeServer); // send an NTP packet to a time server

    while (!Udp.parsePacket())
    {
        delay(DELAYINCREMENT);
        delaycount = delaycount + DELAYINCREMENT;
        if (delaycount > WAITLIMIT)
        {
            Serial.print(++count);
            Serial.print("  No Packet in ");
            Serial.print(WAITLIMIT);
            Serial.println(" ms");
            break;
        }
    }

    if (delaycount <= WAITLIMIT)
    {
        Serial.print("\nSent ");
        Serial.print(++count);
        Serial.print("  Packet ");
        Serial.print(++rcvCount);
        Serial.print(" Received in ");
        Serial.print(delaycount);
        Serial.println(" ms");

        // We've received a packet, read the data from it
        Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

        //the timestamp starts at byte 40 of the received packet and is four bytes,
        // or two words, long. First, esxtract the two words:
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

        // combine the four bytes (two words) into a long integer
        // this is NTP time (seconds since Jan 1 1900):
        unsigned long secsSince1900 = highWord << 16 | lowWord;

        memset(outArr, 0, sizeof(outArr));
        // move ahead to next minute, we'll have to finshis this one
        // out to hit the next minute at the 00 second mark

        secsSince1900 = secsSince1900 + 60;

        secsLeft = computeDate(outArr, secsSince1900);
#ifdef TEST
        Serial.print("\nSeconds: ");
        Serial.print(secsLeft);
        printTestResults(outArr);
    
#else 
        modulateOutArray modulate;

        isRunning = modulate.begin(outArr[Hour]);
        if (isRunning) {
            modulate.doList(outArr);
        }
        else {
            modulate.stopModulator();
        }
        delay(LOOPDELAY);
#endif
    }
    // wait REQUESTDELAY mS before asking for the time again
    delay(REQUESTDELAY);
    // 
    //  Off to output method
    //  if time is between 10PM local and 2AM local, 
    //  engage oscillator and flag in a static value.
    //  when time goes outside these values, turn flag off.
    //  when time is within this value, output method will
    //  delay modulating tone until the next 00 second (computed from
    //  sesLeft) and will then start modulating through the output array for 
    //  a second at a time.  (ToDo - need to set the marker values in the array
    //  to something).  After cycling through the array once, return and invoke 
    //  the 2-minute timer in LOOPDELAY.
    //  Easy.
    //
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress &address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;		  // Stratum, or type of clock
    packetBuffer[2] = 6;		  // Polling Interval
    packetBuffer[3] = 0xEC;		  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); //NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

void printWiFiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
