classdef UnitTests < matlab.unittest.TestCase    

   %% Test function
    methods(Test)
        function testGroupData(testCase)
            cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
            addpath(genpath("uff/matlab"))
            addpath(genpath("uff/install/bin"))
            grp_data=uff.GroupData();

            tmp=13.13;
            grp_data.group_timestamp=tmp;
            testCase.verifyEqual(grp_data.group_timestamp,tmp)

            tmp=double([1.2,.2,3.2,42,59]);
            grp_data.sequence_timestamps=tmp;
            testCase.verifyEqual(grp_data.sequence_timestamps,tmp)

            tmp={[1.1 1.2 1.3] [2.1 2.2 2.3 2.4 2.5] [3.1] [4.1 4.2 4.3 4.4]};
            grp_data.event_timestamps=tmp;
            testCase.verifyEqual(grp_data.event_timestamps,tmp)

            tmp=int16([1+i,2,3]);
            grp_data.raw_data=tmp;
            testCase.verifyEqual(grp_data.raw_data,tmp)

            tmp=uint8(42);
            grp_data.size_of_data_type=tmp;
            testCase.verifyEqual(grp_data.size_of_data_type,tmp)

            tmp=uff.GroupData.DataType.INT16;
            grp_data.data_type=tmp;
            testCase.verifyEqual(grp_data.data_type,tmp)
            
            
            MexUFF('unlock')
            clear all
        end
        
        function testGroup(testCase)
            cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
            addpath(genpath("uff/matlab"))
            addpath(genpath("uff/install/bin"))
            grp = uff.Group();

            % str = "Hello World" Does NOT work!
            str = 'Hello World';
            grp.description = str;
            testCase.verifyEqual(grp.description,str)

            MexUFF('unlock')
            clear all
        end
        
        function testVersion(testCase)
            cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
            addpath(genpath("uff/matlab"))
            addpath(genpath("uff/install/bin"))
            version = uff.Version();            
            
            testCase.verifyEqual(version.major,uint16(0))
            testCase.verifyEqual(version.minor,uint16(0))
            testCase.verifyEqual(version.patch,uint16(0))

            tmp = uint16(42);
            version.major = tmp;
            testCase.verifyEqual(version.major,tmp)
            
            version.minor = tmp;
            testCase.verifyEqual(version.minor,tmp)
            
            version.patch = tmp;
            testCase.verifyEqual(version.patch,tmp)

            MexUFF('unlock')
            clear all
        end
        
        function testDataset(testCase)
            cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
            addpath(genpath("uff/matlab"))
            addpath(genpath("uff/install/bin"))
            dataset = uff.Dataset();
            version = uff.Version(13,42,56);
            
            testCase.verifyNotEqual(dataset.version,version)
            
            dataset.version = version;
            testCase.verifyEqual(dataset.version,version)

            MexUFF('unlock')
            clear all
        end
        
        function testPtr(testCase)
            cd 'E:\Users\Matlab\Documents\MATLAB\TGY_Workspace\PythonWS'
            addpath(genpath("uff/matlab"))
            addpath(genpath("uff/install/bin"))
            acq = uff.Acquisition();
            grp_data=uff.GroupData();
            grp=uff.Group();
            
            testCase.verifyEqual(length(acq.groups),0)
            acq.addGroup()
            testCase.verifyEqual(length(acq.groups),1)
            acq.addGroup(grp)
            testCase.verifyEqual(length(acq.groups),2)
            try
                acq.addGroup(grp)
            catch exceptions
                testCase.verifyEqual(exceptions.identifier, 'MATLAB:unexpectedCPPexception')
            end
            testCase.verifyEqual(length(acq.groups),2)
            
            testCase.verifyEmpty(grp_data.group)
            grp_data.setGroup(grp, acq);
            
            grp.sampling_type = uff.Group.SamplingType.RF;
            testCase.verifyEqual(grp_data.group.sampling_type,uff.Group.SamplingType.RF)

            acq.delGroup(1)
            testCase.verifyEqual(length(acq.groups),1)
            acq.delGroup(1)
            testCase.verifyEqual(length(acq.groups),0)
            
            try
                acq.delGroup(1)
            catch exceptions
                testCase.verifyEqual(exceptions.identifier, 'MATLAB:unexpectedCPPexception')
            end

            g=grp_data.group;
            g.description = 'crash';
            testCase.verifyEqual(grp.description,'crash')
            clear grp
            try
                g.description = 'crash';
            catch exceptions
                testCase.verifyEqual(exceptions.identifier, 'mexplus:session:notFound')
            end

            MexUFF('unlock')
            clear all 
        end
    end
end
