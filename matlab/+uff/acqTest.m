acqObj = uff.Acquisition();
acqObj.sound_speed = 1500;
ptr = calllib('libMatlabCppGlueAcquisition', ['acquisition_' 'sound_speed'], acqObj.id);
ptr.setdatatype('doublePtr', 1)
assert(ptr.Value == 1500)
ptr.Value = 1540;
assert(1540 == acqObj.sound_speed)

g = uff.Group();
g.description = 'my description 0';
g1 = uff.Group();
g1.description = 'my description 1';
% return
acqObj.groups = [g g1];
assert(acqObj.groups(1) == g)
assert(acqObj.groups(2) == g1)


ev = uff.Event();
ev1 = uff.Event();
g.sequence = [ev ev1];
assert(g.sequence(1) == ev)
assert(g.sequence(2) == ev1)
