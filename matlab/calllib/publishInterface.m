pubPath = 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS\uff\matlab\calllib\matlabInterface'; 

if ~isfolder(pubPath)
    mkdir(pubPath)
end

cd(pubPath)
mex -setup cpp

productPath  = 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS\uff\matlab\calllib\'; 
libPath = 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS\uff\build\Matlab\Debug';
% productPath = fullfile(matlabroot,'extern','examples','cpp_interface');
% libPath = fullfile(productPath,'win64','microsoft');

cppFile = 'group_data.cpp';

cppPath = productPath;
uffPath = 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS\uff';
uffBuildPath = 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS\uff\build';
iPath = [cppPath pathsep uffPath pathsep uffBuildPath];
iPath = {cppPath uffPath uffBuildPath};

libFile = 'Uff_Matlab.lib';

myPkg = 'myPkg';
