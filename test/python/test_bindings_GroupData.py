import pyurx as urx
import numpy as np
from time import perf_counter
import gc

import unittest
from parameterized import parameterized


class TestBindingsGroupData(unittest.TestCase):

    # def testHugeDataTransform(self):
    #     testName = "Do computation on huge data vector"
    #     print("\n--Test %s BEGIN--" % testName)

    #     data_size = 100000000
    #     group_data = urx.GroupData()
    #     group_data.raw_data = np.array(range(data_size), dtype=np.float32)
    #     ref = group_data.raw_data
    #     tic = perf_counter()
    #     ref /= 20
    #     toc = perf_counter() - tic

    #     print(f"  Dividing 400MB np.array by 20 computing time: {toc:.3f}")

    #     print("--Test %s END--" % testName)

    # @parameterized.expand([
    #     ["Complex 64", np.complex64],
    #     ["Complex 128", np.complex128]
    # ])
    # def testVariantVectorFloatComplex(self, testName, numpyType):
    #     print("\n--Test %s BEGIN--" % testName)

    #     group_data = urx.GroupData()
    #     self.assertEqual(len(group_data.raw_data), 0)

    #     group_data = urx.GroupData()
    #     group_data.raw_data = numpyType()
    #     self.assertEqual(len(group_data.raw_data), 0)

    #     group_data.raw_data = numpyType([1.+2.j, 2.+3.j])
    #     self.assertEqual(len(group_data.raw_data), 2)

    #     group_data.raw_data = np.array([1.+2.j, 2.+3.j], dtype=numpyType)
    #     self.assertEqual(len(group_data.raw_data), 2)

    #     ref = group_data.raw_data
    #     self.assertTrue(np.array_equal(np.array(
    #         [1.+2.j, 2.+3.j], dtype=numpyType), ref))
    #     self.assertTrue(np.array_equal(np.array(
    #         [[1, 2], [2, 3]], dtype=numpyType), group_data.raw_data))

    #     ref[0] = 45.+67j
    #     self.assertTrue(np.array_equal(
    #         group_data.raw_data.view(numpyType)[:, 0], ref))

    #     ref += 1
    #     self.assertTrue(np.array_equal(ref, np.array(
    #         [46.+67.j, 3.+3.j], dtype=numpyType)))
    #     self.assertTrue(np.array_equal(
    #         group_data.raw_data.view(numpyType)[:, 0], ref))

    #     print("--Test %s END--" % testName)

    def testGroupData(self):
        testName = "GroupData binding"
        print("\n--Test %s BEGIN--" % testName)

        # Check default CTOR
        group_data = urx.GroupData()
        self.assertRaises(
            RuntimeError, lambda group_data: group_data.group, group_data)
        # self.assertTrue(group_data.raw_data, [])
        self.assertEqual(group_data.group_timestamp, urx.DoubleNan())
        self.assertEqual(group_data.sequence_timestamps, urx.VecFloat64())
        self.assertEqual(group_data.event_timestamps, urx.VecVecFloat64())

        # Check copy CTOR and referencing object
        group_data_2 = urx.GroupData(group_data)
        # self.assertEqual(group_data, group_data_2)
        group_data_2.group_timestamp = urx.DoubleNan(0)
        self.assertNotEqual(group_data, group_data_2)
        group_data_ref = group_data
        group_data_ref.group_timestamp = urx.DoubleNan(0)
        self.assertEqual(group_data, group_data_ref)
        group_data_2 = urx.GroupData(group_data)

        # No CTOR with parameters to test

        # Reference is possible for group_timestamp (DoubleNan)
        self.assertEqual(group_data.group_timestamp, 0)
        group_timestamp_ref = group_data.group_timestamp
        group_timestamp_ref.value = 42
        self.assertEqual(group_data.group_timestamp,
                         group_timestamp_ref)
        self.assertNotEqual(group_data, group_data_2)
        # Check affectation
        group_data.group_timestamp = urx.DoubleNan(42)
        self.assertEqual(group_data.group_timestamp,
                         group_timestamp_ref)
        self.assertEqual(group_data.group_timestamp, 42)
        self.assertEqual(group_data, group_data_2)

        # Reference is possible for sequence_timestamps (VecFloat64)
        group_data.sequence_timestamps = urx.VecFloat64([1, 2, 3, 4.56])
        self.assertEqual(group_data.sequence_timestamps, [1, 2, 3, 4.56])
        sequence_timestamps_ref = group_data.sequence_timestamps
        sequence_timestamps_ref = [11, 12.34]
        self.assertEqual(group_data.sequence_timestamps,
                         sequence_timestamps_ref)
        self.assertNotEqual(group_data, group_data_2)
        # Check affectation
        group_data.sequence_timestamps = [1, 2, 3, 4.56]
        self.assertEqual(group_data.sequence_timestamps,
                         sequence_timestamps_ref)
        self.assertEqual(group_data.sequence_timestamps, [1, 2, 3, 4.56])
        self.assertEqual(group_data, group_data_2)

        # Reference is possible for event_timestamps (VecVecFloat64)
        group_data.event_timestamps = urx.VecVecFloat64(
            [[1, 2, 3, 4.56], [7.8, 9]])
        self.assertEqual(group_data.event_timestamps,
                         [[1, 2, 3, 4.56], [7.8, 9]])
        event_timestamps_ref = group_data.event_timestamps
        event_timestamps_ref[0] = [11, 12.34]
        self.assertEqual(group_data.event_timestamps,
                         event_timestamps_ref)
        self.assertNotEqual(group_data, group_data_2)
        # Check affectation
        group_data.event_timestamps = [[1, 2, 3, 4.56], [7.8, 9]]
        self.assertEqual(group_data.event_timestamps,
                         event_timestamps_ref)
        self.assertEqual(group_data.event_timestamps,
                         [[1, 2, 3, 4.56], [7.8, 9]])
        self.assertEqual(group_data, group_data_2)

        # group is a pointer and will always be shared
        group = urx.Group()
        group_data.group = group
        group_data_2.group = group
        self.assertEqual(group_data.group, group)
        group_ref = group_data.group
        group_ref.description = "BMode"
        self.assertEqual(group_data.group, group_ref)
        self.assertNotEqual(group_data, group_data_2)
        # Check affectation
        group_data.group = group
        self.assertEqual(group_data.group, group_ref)
        self.assertEqual(group_data.group, group)
        self.assertEqual(group_data, group_data_2)
        del group
        gc.collect()
        self.assertRaises(
            RuntimeError, lambda group_data: group_data.group, group_data)
        self.assertRaises(
            RuntimeError, lambda group_data: group_data.group, group_data_2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
