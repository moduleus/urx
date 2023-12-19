clear all
dataset = urx.Dataset();

dataset.version.minor = 123;
dataset.version.major = 456;
dataset.version.patch = 789;

dataset.acquisition.authors = 'AuthOr';
dataset.acquisition.description = '康Description🐜';
dataset.acquisition.local_time = '2019-06-15T15:53:00';
dataset.acquisition.country_code = 'FR';
dataset.acquisition.system = '???';
dataset.acquisition.sound_speed = 1500.;

%probe = urx.Probe();



acqObj = urx.Acquisition();

acqObj.local_time = 'coucou';
ptr = urx.LibBinding.getInstance().call('urx_Acquisition_local_time', acqObj.id);
ptr_val = urx.LibBinding.getInstance().call('std_string_get', ptr);
assert(strcmp(ptr_val, 'coucou'))
urx.LibBinding.getInstance().call('std_string_set', ptr, 'foobar');
assert(strcmp('foobar', acqObj.local_time))

acqObj.sound_speed = 1500;
ptr = urx.LibBinding.getInstance().call('urx_Acquisition_sound_speed', acqObj.id);
ptr.setdatatype('doublePtr', 1)
assert(ptr.Value == 1500)
ptr.Value = 1540;
assert(1540 == acqObj.sound_speed)
% return

g = urx.Group();
g.description = 'my description 0';
g1 = urx.Group();
g1.description = 'my description 1';
% return
acqObj.groups = [g g1];
assert(acqObj.groups(1) == g)
assert(acqObj.groups(2) == g1)
% return;


ev = urx.Event();
ev1 = urx.Event();
g.sequence = [ev ev1];
assert(g.sequence(1) == ev)
assert(g.sequence(2) == ev1)

rxs = urx.ReceiveSetup();
rxs1 = urx.ReceiveSetup();
%acqObj.unique_receive_setups = [rxs rxs1];
ev.receive_setup = rxs;
