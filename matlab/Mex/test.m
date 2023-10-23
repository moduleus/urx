cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
%addpath(genpath("uff/install"))
addpath(genpath("uff/matlab"))
addpath(genpath("uff/install/bin"))
addpath(genpath("uff/install/lib"))
grp_data=uff.GroupData()

grp_data.sequence_timestamps=double([1.2,.2,3.2,42,59]);
grp_data.sequence_timestamps

%tmp=double([1.1 1.2 1.3 -1 -1;2.1 2.2 2.3 2.4 2.5;3.1 -1 -1 -1 -1;4.1 4.2 4.3 4.4 -1])
tmp={[1.1 1.2 1.3] [2.1 2.2 2.3 2.4 2.5] [3.1] [4.1 4.2 4.3 4.4]};
grp_data.event_timestamps=tmp;
grp_data.event_timestamps

grp_data.raw_data=int16([1+i,2,3]);
grp_data.raw_data

grp_data.size_of_data_type=uint8(42);
grp_data.size_of_data_type

grp_data.data_type=uff.GroupData.DataType.INT16;
grp_data.data_type

grp = uff.Group()

% str = "Hello World" Does NOT work!
str = 'Hello World'
class(str)
grp.description = str;
grp.description

MexUFF('unlock')
clear all