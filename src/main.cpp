// The MIT License (MIT)
//
// Copyright (c) 2015 THINGER LTD
// Author: alvarolb@gmail.com (Alvaro Luis Bustamante)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "thinger/thinger.h"

#include "LoRa.h"
#include <stdio.h>
#include<iostream>
#include<string>
#include<fstream>
using namespace std;

#define USER_ID             "ifwat"
#define DEVICE_ID           "linuxifwat"
#define DEVICE_CREDENTIAL   "linuxifwat"

float Q1_Nitrogen=0;
float Q1_Phosphorus=0;
float Q1_Potassium=0;
float Q2_Nitrogen =0;
float Q2_Phosphorus=0;
float Q2_Potassium=0;
float Q3_Nitrogen =0;
float Q3_Phosphorus=0;
float Q3_Potassium=0;
float Q4_Nitrogen =0;
float Q4_Phosphorus=0;
float Q4_Potassium=0;
float temperature=0;
float humidity=0;
float lux=0;

float Q1_sensorN={0,0};
float Q1_sensorP={0,0};
float Q1_sensorK={0,0};
float Q2_sensorN={0,0};
float Q2_sensorP={0,0};
float Q2_sensorK={0,0};
float Q3_sensorN={0,0};
float Q3_sensorP={0,0};
float Q3_sensorK={0,0};
float Q4_sensorN={0,0};
float Q4_sensorP={0,0};
float Q4_sensorK={0,0};

int Q1_sensorN_status1=0;
int Q1_sensorP_status1=0;
int Q1_sensorK_status1=0;
int Q1_sensorN_status2=0;
int Q1_sensorP_status2=0;
int Q1_sensorK_status2=0;


void setup() {

  printf("LoRa Receiver\n");
  LoRa.setPins(10, 6, 24);
  
  if (!LoRa.begin(433E6, 0)) {
    printf("Starting LoRa failed!\n");
    while (1);
  }
  LoRa.setSyncWord(0xF3);
  LoRa.enableCrc();
  printf("Init LoRa Done !!\n");
}

int main(int argc, char *argv[])
{
    setup();

    while(1){

        int packetSize = LoRa.parsePacket();

        if (packetSize) 
        {
            // received a packet
            printf("Received packet '");
            char buffer[100];
            int i = 0;
            
            // read packet
            while (LoRa.available()) 
            {
                buffer[i] = LoRa.read();
                printf("%c", buffer[i]);
                i++;
            }

            int j=0;

            int j = 0;
            char* token = strtok(buffer, "|");
            while (token != NULL) { 
            data_packet_str[i] = token;
            data_packet_int[i] = data_packet_str[i].toInt();
            Serial.print(data_packet_int[i]);
            j++; 
            token = strtok(NULL, "|");

            //Sensor 1
            if(data_packet_int[0]==1 && data_packet_int[1]==4)
            {
                Q1_sensorN[0]=data_packet_int[2];  //N
                Q1_sensorN_status1=1;
            }

            if(data_packet_int[0]==1 && data_packet_int[1]==5)
            {
                Q1_sensorP[0]=data_packet_int[2];  //P
                Q1_sensorP_status1=1;
            }

            if(data_packet_int[0]==1 && data_packet_int[1]==6)
            {
                Q1_sensorK[0]=data_packet_int[2];  //K
                Q1_sensorK_status1=1;
            }

            //Sensor 2
            if(data_packet_int[0]==2 && data_packet_int[1]==4)
            {
                Q1_sensorN[1]=data_packet_int[2];  //N
                Q1_sensorN_status2=1;
            }

            if(data_packet_int[0]==2 && data_packet_int[1]==5)
            {
                Q1_sensorP[1]=data_packet_int[2];  //P
                Q1_sensorP_status2=1;
            }

            if(data_packet_int[0]==2 && data_packet_int[1]==6)
            {
                Q1_sensorK[1]=data_packet_int[2];  //K
                Q1_sensorK_status2=1;
            }

            //Sensor 9
            if(data_packet_int[0]==9 && data_packet_int[1]==1)
            {
                temperature=data_packet_int[2];
            }

            if(data_packet_int[0]==9 && data_packet_int[1]==2)
            {
                humidity=data_packet_int[2];
            }

            //Sensor 10
            if(data_packet_int[0]==10 && data_packet_int[1]==3)
            {
                lux=data_packet_int[2];
            }
          
        }

        // Averaging Quadrant 1 sensors values

        if(Q1_sensorN_status1=1 && Q1_sensorN_status2=1)
        {
            Q1_Nitrogen = (Q1_sensorN[0]+Q1_sensorN[1])/2;
            Q1_sensorN_status1=0;
            Q1_sensorN_status2=0;
        }

        if(Q1_sensorP_status1=1 && Q1_sensorP_status2=1)
        {
            Q1_Phosphorus = (Q1_sensorP[0]+Q1_sensorP[1])/2;
            Q1_sensorP_status1=0;
            Q1_sensorP_status2=0;
        }

        if(Q1_sensorK_status1=1 && Q1_sensorK_status2=1)
        {
            Q1_Potassium = (Q1_sensorK[0]+Q1_sensorK[1])/2;
            Q1_sensorK_status1=0;
            Q1_sensorK_status2=0;
        }


        
        
        thinger_device thing(USER_ID, DEVICE_ID, DEVICE_CREDENTIAL);

        // define thing resources here. i.e, this is a sum example
        thing["Greenhouse3A"] = [](pson& in, pson& out)
        {
            out["Q1_Nitrogen"] = Q1_Nitrogen;
            out["Q1_Phosphorus"] = Q1_Phosphorus;
            out["Q1_Potassium"] = Q1_Potassium;
            out["Q1_Temperature"] = temperature;
            out["Q1_Humidity"] = humidity;
            out["Q1_Lux"] = lux;
        }

        thing.handle();




    }
    
}

