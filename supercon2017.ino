#include <SPI.h>
#include <ILI9341_t3.h>
#include <font_GeorgiaItalic.h> // https://github.com/PaulStoffregen/ILI9341_fonts
#include <font_LiberationMonoBold.h>
#include <XPT2046_Touchscreen.h>

#define CS_PIN  8
#define TFT_DC  9
#define TFT_CS 10

// Use hardware SPI (#13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
XPT2046_Touchscreen ts(CS_PIN);

int mode; //0=paul, 1=bsod, 2=bsodtouch, 3=metal
int timer;

void setup() {
  tft.begin();
  tft.setRotation(1);
  ts.begin();
  Serial.begin(9600);
  Serial.println("Supercon 2017 Badge"); 
  paul();
  teensy();
  mode = 0;  
}


void loop(void) {
  boolean istouched = ts.touched();
  if (istouched) {
    TS_Point p = ts.getPoint();
    Serial.print("touch");
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.println(p.y);
    if (mode == 0 || mode == 3) {
      bsod();
      mode = 1;
      timer = 0;
    } else if (mode == 2) {
      metal();
      teensy();
      mode = 3;
    }
  }
  if (mode == 1) {
    timer++;
    if (timer > 100) {
      mode = 2;
      timer = 0;
      tft.print("M");
    }
  }
  if (mode == 2) {
    timer++;
    if (timer > 20) {
      mode = 0;
      paul();
      teensy();
    }
  }

  
  delay(50);
  //asm("wfi");
}

void bsod() {
  tft.fillScreen(ILI9341_BLUE);
  tft.setTextColor(ILI9341_WHITE);
  tft.setFont(LiberationMono_9_Bold);
  tft.setCursor(20, 40);
  tft.print("An error has occured. To continue:");
  tft.setCursor(20, 75);
  tft.print("Press Enter to return to Windows, or");
  tft.setCursor(20, 100);
  tft.print("Press CTRL+ALT+DEL to restart your");
  tft.setCursor(20, 115);
  tft.print("computer.  If you do this, you will");
  tft.setCursor(20, 130);
  tft.print("lose any unsaved information.");
  tft.setCursor(20, 160);
  tft.print("Error: 0E : 016F : BFF9B3D4");
  tft.setCursor(60, 208);
  tft.print("Press any key to continue _");
}

void paul() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(20, 30);
  tft.setFont(Georgia_96_Italic);
  tft.print("Paul");
}

void metal() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(40, 36);
  tft.setFont(Georgia_72_Italic);
  tft.print("Metal");
}

extern "C" const unsigned char teensy32[];

void teensy() {
  const unsigned char *p = teensy32 + 18;
  for (int y=0; y < 72; y++) {
    for (int x=0; x < 151; x++) {
      unsigned char r = *p++;
      unsigned char g = *p++;
      unsigned char b = *p++;
      p++;
      uint16_t c = ILI9341_t3::color565(r, g, b);
      tft.drawPixel(x+85, y+152, c);
    }
  }
}



/*
#! /usr/bin/perl

binmode IN, ":bytes";
$file = $ARGV[0];

foreach $file (@ARGV) {
        open(IN, $file) or die "Can't open $file: $!\n";

        $count = 0;

        while (read(IN, $byte, 1) == 1) {
                $n = ord($byte);
                printf "0x%02X,", $n;
                $count++;
                print "\n" if ($count & 15) == 0;
        }
        print "\n";
}

*/
