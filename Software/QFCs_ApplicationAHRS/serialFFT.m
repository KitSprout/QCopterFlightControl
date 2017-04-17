clear;

dataLens = 15;
dataType = 'int16';

s = kSerial(115200, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

fig = figure(1);
f = struct;
f.fig = subplot(1, 1, 1);
f.length = 128;
f.window = [-10, 170, 0, 1.2];
f = kSerialFFT(f, 0, 0, 'init');

while ishandle(fig)
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

        lens = f.length;
        f = kSerialFFT(f, data(3, end - lens + 1 : end), freq, 0);
        fprintf('[%05i][%02i][%02i:%02i:%02i][%4.0fHz] Gyr[%6.0f, %6.0f, %6.0f] Acc[%6.0f, %6.0f, %6.0f] Mag[%6.0f, %6.0f, %6.0f] Att[%6.3f, %6.3f, %6.3f, %6.3f]\n', seNum, packetLens, tt, freq, gyr, acc, mag, q);
    end
end

s.close();
