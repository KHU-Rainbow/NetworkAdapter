all: NW_YJ

NW_YJ: getmac.o parser.o main.o
	g++ -o NW_YJ getmac.o parser.o main.o -std=c++11 -lpcap -lcurl -lwiringPi

getmac.o: getmac.cpp pkt.h
	g++ -c -o getmac.o getmac.cpp -std=c++11 -lpcap -lcurl -lwiringPi

parser.o: parser.cpp pkt.h
	g++ -c -o parser.o parser.cpp -std=c++11 -lpcap -lcurl -lwiringPi

main.o: main.cpp pkt.h
	g++ -c -o main.o main.cpp -std=c++11 -lpcap -lcurl -lwiringPi

clean:
	rm -f NW_YJ *.o
