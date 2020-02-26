# px4-rpi-drone-swarm

sdptool search SP

sudo hciconfig 0 sspmode 0

sudo rfcomm connect hci0 B8:27:EB:89:C5:44 8

1. 9C:E0:63:BD:A8:88 `Note 8 (Android)`

2. B8:27:EB:89:C5:44 `RPi (Raspberian)`

3. A4:17:31:36:3E:16 `Notebook (Linux Ubuntu 19.04)`





    //quint128 buffer = QBluetoothUuid(...).toUInt128();
    quint128 buffer = QBluetoothUuid(serviceUuid).toUInt128();
    quint128 correctedUuid = {};
    for (int i = 0; i < 16; i++)
        correctedUuid.data[15 - i] = buffer.data[i];
    qDebug() << QBluetoothUuid(correctedUuid);
