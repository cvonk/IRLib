/*
 * Display codes coming from SilverLit remote control
 * Detect an IR receiver such as TSOP38238 to RECV_PIN (see https://learn.adafruit.com/using-an-infrared-library/hardware-needed)
 * Protocol details can be found at http://www.coertvonk.com/technology/embedded/arduino-ir-for-silverlit-rc-vehicles-12970
 * By Coert Vonk, 2015
 */

#include <IRLib.h>

int RECV_PIN = 11;

IRrecv My_Receiver( RECV_PIN );
IRdecode My_Decoder;
unsigned int Buffer[RAWBUF];

void setup()
{
    Serial.begin( 9600 );
    My_Receiver.enableIRIn();
    My_Decoder.UseExtnBuf( Buffer );
}


void loop()
{
    if ( My_Receiver.GetResults( &My_Decoder ) ) {
        My_Receiver.resume();
        if ( My_Decoder.decode() ) {
            unsigned long const data = My_Decoder.value;

            if ( (data & 0b111111100L) != 0b110000000L ) {  // don't display idle codes

                char const * vehicle;
                switch ( data >> 11 ) {
                    case 0: vehicle = "dumptruck "; break;
                    case 1: vehicle = "flatbed "; break;
                    case 2: vehicle = "bulldozer "; break;
                    case 3: vehicle = "crane "; break;
                }
                Serial.print( vehicle );

                if ( bitRead( data, 8 ) == 0 ) Serial.print( "backward " );
                if ( bitRead( data, 7 ) == 0 ) Serial.print( "forward " );
                if ( bitRead( data, 6 ) == 1 ) Serial.print( "right " );
                if ( bitRead( data, 5 ) == 1 ) Serial.print( "left " );
                if ( bitRead( data, 4 ) == 1 ) Serial.print( "down " );
                if ( bitRead( data, 3 ) == 1 ) Serial.print( "up " );
                if ( bitRead( data, 2 ) == 1 ) Serial.print( "light " );
                Serial.println();
            }
        }
    }
}
