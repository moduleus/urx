
cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
addpath(genpath("uff/matlab"))
addpath(genpath("uff/install/bin"))
acq = uff.Acquisition();
grp_data=uff.GroupData();
grp=uff.Group();

tmp=13.13;
grp_data.group_timestamp=tmp;

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
grp.sampling_type = uff.Group.SamplingType.RF

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


MexUFF('unlock')
clear all