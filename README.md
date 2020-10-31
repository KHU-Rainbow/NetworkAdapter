# Rainbow - Network Adapter

## Project Explanation
### 인터넷 사용 감지
- 사용자가 비행기 모드를 해제하고 네트워크를 사용하는 순간 알림.
- 핸드폰에서 주변 AP(Access Point)를 찾는 패킷인 Probe Request 패킷을 전송함.
- 네트워크 어댑터를 Monitor Mode로 전환하여 Probe Packet들을 캡쳐함.
- 캡쳐한 패킷들 중에서, 사용자의 핸드폰 MAC 주소와 일치하는 패킷을 골라냄.
- 해당 패킷의 RSSI (신호세기)값이 일정 수준 이상인 경우, 이를 DB에 전송함.

## Build Method
- **Basic Configuration for Network Adapter**
```
sudo ifconfig [어댑터명 ex)wlx9cefd5fbc93c] down
sudo ip link [어댑터명 ex)wlx9cefd5fbc93c] name wlan0

sudo iwconfig wlan0 mode monitor
sudo ifconfig wlan0 up

```
<img src="https://user-images.githubusercontent.com/58834907/97774821-8419d400-1b9e-11eb-9e95-7b389b2d0c2b.PNG" width="90%"></img>

- **Execute Program**
```
make
sudo ./NW_YJ <interface> <mac address>
sudo ./NW_YJ wlan0 aa:bb:cc:dd:ee:ff
```
<img src="https://user-images.githubusercontent.com/58834907/97774846-a0b60c00-1b9e-11eb-8aa7-966a3d615e45.PNG" width="90%"></img>
