# Самоорганизующаяся система управления беспилотными летательными аппаратами

![](workgif.gif)


<details>
  <summary>Координаты для Энергетика</summary>
    
Home
    
lat 440767084
lon 430877167
alt 530000

точка 1

lat 440769288
lon 430879335
alt 540000

точка 2

lat 440764170
lon 430876500
alt 540000
</details>

<details>
  <summary>Симулятор</summary>
    
export PX4_HOME_ALT=530

export PX4_HOME_LON=43.0877167

export PX4_HOME_LAT=44.0767085

export HEADLESS=1

make px4_sitl_default jmavsim

</details>

<details>
  <summary>Bluetooth tweaks</summary>
    
sdptool search SP

/etc/bluetooth/

sudo hciconfig 0 sspmode 0

sudo rfcomm connect hci0 B8:27:EB:89:C5:44 8

1. 9C:E0:63:BD:A8:88 `Note 8 (Android)`

2. B8:27:EB:89:C5:44 `RPi (Raspberian)`

3. A4:17:31:36:3E:16 `Notebook (Linux Ubuntu 19.04)`

4. 00:1A:7D:11:A1:6A  `HPN (Linux Ubuntu 18.04)` or `HPN (Windows 10)`
    
</details>
