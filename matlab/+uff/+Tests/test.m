
cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
addpath(genpath("uff/matlab"))
addpath(genpath("uff/install/bin"))
dataset = uff.Dataset();
version = uff.Version();
version.major = uint16(13);
version.minor = uint16(42);
version.patch = uint16(56);

assert(dataset.version~=version)

dataset.version = version;
assert(isequal(dataset.version,version))

MexUFF('unlock')
clear all