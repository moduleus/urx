cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
addpath(genpath(pwd))
toto=uff.GroupData()

toto.sequence_timestamps=double([1.2,.2,3.2,42,59]);
toto.sequence_timestamps

toto.raw_data=int16([1+i,2,3]);
toto.raw_data

toto.size_of_data_type=uint8(42);
toto.size_of_data_type

% a=MexGroupData('set_raw_data', toto.id_, int32([1+i,2,3]))
% c=MexGroupData('get_raw_data', toto.id_)
MexGroupData('unlock')
clear all