sv2d = uff.StdVector('double', [], [], 2)
sv = uff.StdVector('double')
sv.pushBack(1)
sv2d.pushBack(sv)
sv.pushBack(2)
sv2d.pushBack(sv)
sv2d.updateFromCpp();
sv2d.objects(1).getFromCpp
sv2d.objects(2).getFromCpp
