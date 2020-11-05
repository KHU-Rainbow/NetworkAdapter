#include "pkt.h"
#include "radiotap_header.h"
#include <pcap.h>
#include <chrono>
#include <curl/curl.h>

int packet_capture(char* dev, char* node_mac, char* my_device_mac){
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "pcap_open_live(%s) return nullptr - %s\n", dev, errbuf);
        return -1;
    }

    while (true) {
        struct pcap_pkthdr* header;
        const u_char* packet;
        int res = pcap_next_ex(handle, &header, &packet);
        if (res == 0) continue;
        if (res == -1 || res == -2) {
            printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(handle));
            break;
        }

        parsing(packet, node_mac, my_device_mac);
    }

    pcap_close(handle);
    return 0;
}


void parsing(const u_char* packet, char* node_mac, char* my_device_mac){
    //packet header setting
    struct radiotap_header *rh = (struct radiotap_header *)packet;
    struct ieee80211_header *ih = (struct ieee80211_header *)(packet + rh->it_length);
    //uint8_t *wlh = (uint8_t *)ih + IEEE_LEN;               //wireless LAN header

    //my_device_mac change to hex
    uint8_t my_mac[6];
    char t[4];

    for(int i = 0; i <6; i++){
        memcpy(t, (my_device_mac +i*3), 3);
        t[3] = '\0';
        *(my_mac +i) = (uint8_t)strtoul(t, NULL, 16);
    }
    //printf("my_mac :  %02x:%02x:%02x:%02x:%02x:%02x   ",my_mac[0], my_mac[1], my_mac[2], my_mac[3], my_mac[4], my_mac[5]);
    
    //Catch Probe_request & parsing
    if( ih->type_subtype == PROBE_REQUEST && rh->it_antenna_signal > 186){
        static int cnt = 0;
        cnt++;
        printf("%3d Probe Request=============================================\n", cnt);

        char device_mac[17]; // enough size
        //%02X : 2 hex code 
        sprintf(device_mac, "%02x:%02x:%02x:%02x:%02x:%02x", ih->add2[0], ih->add2[1], ih->add2[2], ih->add2[3], ih->add2[4], ih->add2[5]);
        printf("device_mac : %s, ", device_mac);

        if((my_mac[0] == ih->add2[0])&&(my_mac[1] == ih->add2[1])&&(my_mac[2] == ih->add2[2])
            &&(my_mac[3] == ih->add2[3])&&(my_mac[4] == ih->add2[4])&&(my_mac[5] == ih->add2[5])){
            printf("\n=== This is from your phone ===\n");
            ledControl();
            //Send Data to Cloud Service 
            //~~
        }

        int rssi = 0;
        if (rh->it_antenna_signal<127)
            rssi= rh->it_antenna_signal -1;
        else
            rssi= rh->it_antenna_signal -255 -1;
        printf("rssi : %d, ", rssi);


        const auto p1 = chrono::system_clock::now();
        int timestamp = chrono::duration_cast<chrono::seconds>(p1.time_since_epoch()).count();
        printf("timestamp : %d \n", timestamp);
        printf("\n");
    }
}


int ledControl(){
    if (wiringPiSetup () == -1)
        return 1 ;
        
    const int LED6 = 10;
    const int LED7 = 7;
    const int LED10 = 9;
    const int LED11 = 11;

    pinMode (LED6, OUTPUT) ;
    pinMode (LED7, OUTPUT) ;
    pinMode (LED10, OUTPUT) ;
    pinMode (LED11, OUTPUT) ;

    for (int i = 0; i < 3; i ++){
        digitalWrite (LED6, 1) ; // On
        digitalWrite (LED7, 1) ; // On
        digitalWrite (LED10, 1) ; // On
        digitalWrite (LED11, 1) ; // On

        delay (500) ; // ms

        digitalWrite (LED6, 0) ; // Off
        digitalWrite (LED7, 0) ; // Off
        digitalWrite (LED10, 0) ; // On
        digitalWrite (LED11, 0) ; // On

        delay (500) ;
    }
    return 0;

}