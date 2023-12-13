acqObj = uff.Acquisition();
acqObj.sound_speed = 1500;
ptr = uff.LibBinding.call(['Acquisition_' 'sound_speed'], acqObj.id);
ptr.setdatatype('doublePtr', 1)
assert(ptr.Value == 1500)
ptr.Value = 1540;
assert(1540 == acqObj.sound_speed)
% return

g = uff.Group();
g.description = 'my description 0';
g1 = uff.Group();
g1.description = 'my description 1';
% return
acqObj.groups = [g g1];
assert(acqObj.groups(1) == g)
assert(acqObj.groups(2) == g1)
% return;


ev = uff.Event();
ev1 = uff.Event();
g.sequence = [ev ev1];
assert(g.sequence(1) == ev)
assert(g.sequence(2) == ev1)

rxs = uff.ReceiveSetup();
rxs1 = uff.ReceiveSetup();
acqObj.unique_receive_setups = [rxs rxs1];
ev.receive_setup = rxs;
