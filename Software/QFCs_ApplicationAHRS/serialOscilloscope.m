clear;

dataLens = 15;
dataType = 'int16';

s = kSerial(115200, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

fig = figure(1);
set(fig, 'Position', [100, 140, 1200, 600], 'color', 'w');

figSub = subplot(1, 1, 1);
osc = kSerialOscilloscope();
osc.setWindow(8192*2, -8192*2, 1000);
osc.curveChannel = 1 : 3;
osc.curveColor   = ['r', 'g', 'b'];
osc.curveScale   = [1, 1, 1];
osc.initOscilloscope(figSub);

while ishandle(osc.fig)
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

        osc.updateOscilloscope(s);
        fprintf('[%05i][%02i][%02i:%02i:%02i][%4.0fHz] Gyr[%6.0f, %6.0f, %6.0f] Acc[%6.0f, %6.0f, %6.0f] Mag[%6.0f, %6.0f, %6.0f] Att[%6.3f, %6.3f, %6.3f, %6.3f]\n', seNum, packetLens, tt, freq, gyr, acc, mag, q);
    end
end

s.close();
% s.save2mat('rawData', {'gyr(1:3)', 'acc(4:6)', 'mag(7:9)', 'q((10:13))', 'sec(14)', 'msc(15)'});
