
cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
addpath(genpath("uff/matlab"))
addpath(genpath("uff/install/bin"))
grp_data=uff.GroupData();

tmp_comp=int16([1+i,2,3]);
tmp=int16([1,2,3]);
grp_data.void_raw_data=tmp;


% MexUFF('unlock')
% clear all