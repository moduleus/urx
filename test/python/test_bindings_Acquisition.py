import pyurx as urx
import numpy as np
from time import perf_counter

import unittest
from parameterized import parameterized


class TestBindingsAcquisition(unittest.TestCase):

    def testAcq_GroupData_Group(self):
        testName = "Acquisition & Groups & GroupData binding"
        print("\n--Test %s BEGIN--" % testName)

        acq = urx.Acquisition()
        group_data = urx.GroupData()
        group = urx.Group()
        acq.groups.append(group)
        group_data.group = group

        self.assertEqual(len(acq.groups), 1)
        self.assertEqual(acq.groups[0].description, "")
        self.assertEqual(acq.groups[0].sampling_type,
                         urx.SamplingType.UNDEFINED)
        self.assertEqual(acq.groups[0].data_type, urx.DataType.UNDEFINED)

        acq.groups[0].description = "Hello World"
        self.assertEqual(group.description, "Hello World")

        acq.groups.append(group)
        self.assertEqual(len(acq.groups), 2)
        acq.groups.append(urx.Group())
        self.assertEqual(len(acq.groups), 3)
        acq.groups[2].sampling_type = urx.SamplingType.IQ

        self.assertEqual(acq.groups[0], acq.groups[1])
        self.assertEqual(id(acq.groups[0]), id(acq.groups[1]))
        self.assertNotEqual(acq.groups[0], acq.groups[2])

        import gc
        del group
        gc.collect()
        self.assertEqual(group_data.group.description, "Hello World")

        acq.groups.remove(acq.groups[0])
        self.assertEqual(group_data.group.description, "Hello World")

        acq.groups.remove(acq.groups[0])
        self.assertRaises(
            RuntimeError, lambda group_data: group_data.group, group_data)
        self.assertEqual(acq.groups[0].sampling_type, urx.SamplingType.IQ)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
