
cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
addpath(genpath("urx/matlab"))
addpath(genpath("urx/install/bin"))
acq = urx.Acquisition();
grp_data=urx.GroupData();
grp=urx.Group();

tmp=13.13;
grp_data.group_timestamp=tmp;
grp_data.group=grp;

tmp=int16([1+i,2,3]);
grp_data.raw_data=tmp;

acq.addGroup()
acq.addGroup(grp)
try
    acq.addGroup(grp)
catch exceptions
    disp(exceptions.message)
end
grp.sampling_type
grp.sampling_type = urx.Group.SamplingType.RF

grp_data.setGroup(grp, acq)


acq.delGroup(1)
acq.delGroup(1)

g=grp_data.group
g.description = 'crash'
grp.description
clear grp
try
    g.description = 'crash'
catch exceptions
    disp(exceptions.identifier)
    disp(exceptions.message)
end


MexURX('unlock')
clear all