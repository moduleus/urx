addpath(genpath(pwd))
a=double([1.2,.2,3.2,42,59])
addpath(genpath(pwd))
toto=uff.GroupData()
toto.sequence_timestamps=a
%vector('set_sequence_timestamps',toto.id_,a);
toto.printData()
clear MexGroupData
clear toto
clear