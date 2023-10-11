cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
addpath(genpath(pwd))
toto=uff.GroupData();
toto.sequence_timestamps=double([1.2,.2,3.2,42,59])
[a,b]=MexGroupData('set_raw_data', toto.id_, int32([1+i,2,3]))
c=MexGroupData('get_raw_data', toto.id_)
MexGroupData('unlock')
clear all