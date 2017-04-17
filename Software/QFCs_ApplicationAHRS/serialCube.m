clear;

dataLens = 15;
dataType = 'int16';

s = kSerial(115200, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

fig = figure(1);
set(fig, 'Position', [100, 140, 1200, 600], 'color', 'w');

figSub = subplot(1, 1, 1);
cube = kSerialCube([0, 0, 0], [1.5, 1.5, 0.5], [1.5, 1.5, 1.5]);   % origin, scale, window
cube.initCube(figSub, [315, 30]);           % view

while ishandle(figSub)
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
        att = cube.getAttitude(q);
        tt  = [ fix(sec / 60), mod(sec, 60), fix(msc / 10) ];
        seNum = s.packet.sequenceNum;
        freq = s.getFreq([14, 15], 128, 0.001);

        cube.plotCube([0, 0, 0], q);
        cube.qText.String = num2str([ tt(1), tt(2), tt(3), ...
                                      freq, ...
                                      gyr(1), gyr(2), gyr(3), ...
                                      acc(1), acc(2), acc(3), ...
                                      mag(1), mag(2), mag(3), ...
                                      att(1), att(2), att(3), ...
                                      q(1), q(2), q(3), q(4) ], ...
                                     'TIME [ %02i:%02i:%02i ]\n\nFreq = %6.2fHz\n\ngyr.x = %6.0f\ngyr.y = %6.0f\ngyr.z = %6.0f\n\nacc.x = %6.0f\nacc.y = %6.0f\nacc.z = %6.0f\n\nmag.x = %6.0f\nmag.y = %6.0f\nmag.z = %6.0f\n\natt.x = %7.3f\natt.y = %7.3f\natt.z = %7.3f\n\nq0 = %8.5f\nq1 = %8.5f\nq2 = %8.5f\nq3 = %8.5f');
        fprintf('[%05i][%02i][%02i:%02i:%02i][%4.0fHz] Gyr[%6.0f, %6.0f, %6.0f] Acc[%6.0f, %6.0f, %6.0f] Mag[%6.0f, %6.0f, %6.0f] Att[%7.3f, %8.3f, %8.3f]\n', seNum, packetLens, tt, freq, gyr, acc, mag, att);
    end
end

s.close();
% s.save2mat('rawData', {'gyr(1:3)', 'acc(4:6)', 'mag(7:9)', 'q((10:13))', 'sec(14)', 'msc(15)'});
