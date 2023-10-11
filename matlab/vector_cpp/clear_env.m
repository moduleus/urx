addpath(genpath(pwd))
clear toto
vector('unlock')
clear vector
clear
a=double([1.2,.2,3.2,42,59])
delete uff/matlab/vector/gen/vector*
mex 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS\uff\matlab\vector_cpp\group_data.cpp' ...
    -output 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS\uff\matlab\vector_cpp\gen\vector' ...
    -I"E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS\mexplus-master\include" ...
    -I"E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS\uff" ...
    -I"E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS\uff\build"
addpath(genpath(pwd))
toto=uff.GroupData()
toto.sequence_timestamps=a
%vector('set_sequence_timestamps',toto.id_,a);
toto.printData()