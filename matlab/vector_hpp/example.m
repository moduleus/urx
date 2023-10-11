clear all
addpath(genpath(pwd))
clibgen.generateLibraryDefinition('uff/matlab/vector_hpp/group_data.hpp',"OutputFolder","uff/matlab/vector_hpp/gen")
cd uff/matlab/vector_hpp/gen
build(definegroup_data)
summary(definegroup_data)
cd ../../../..
addpath(genpath(pwd))
gr_data = clib.group_data.GroupData()
clear all
delete uff/matlab/vector_hpp/gen/*