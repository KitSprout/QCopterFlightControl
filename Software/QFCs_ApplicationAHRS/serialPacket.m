clear;

dataLens = 15;
dataType = 'int16';

s = kSerial(115200, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

for i = 1 : 2000
%while true
    [packetData, packetLens] = s.packetRecv(dataLens, dataType);
    if packetLens > 0
        s.updateBuffer(packetData, packetLens);     % record data
        data = s.getBuffer();
        gyr = data( 1:  3, end);
        acc = data( 4:  6, end);
        mag = data( 7:  9, end);
        q   = data(10: 13, end) / 10000;
        sec = data(14, end);
        msc = data(15, end);
        tt  = [ fix(sec / 60), mod(sec, 60), fix(msc / 10) ];
        seNum = s.packet.sequenceNum;
        freq = s.getFreq([14, 15], 128, 0.001);

        fprintf('[%05i][%02i][%02i:%02i:%02i][%4.0fHz] Gyr[%6.0f, %6.0f, %6.0f] Acc[%6.0f, %6.0f, %6.0f] Mag[%6.0f, %6.0f, %6.0f] q[%6.3f, %6.3f, %6.3f, %6.3f]\n', seNum, packetLens, tt, freq, gyr, acc, mag, q);
    end
end

s.close();
s.save2mat('rawData', {'gyr(1:3)', 'acc(4:6)', 'mag(7:9)', 'q((10:13))', 'sec(14)', 'msc(15)'});
