import pyurx as urx
import numpy as np
from time import perf_counter
import gc

import unittest
from parameterized import parameterized


class TestBindingsGroupData(unittest.TestCase):

    @parameterized.expand([
        ["Do computation on data vector 100", 100],
        ["Do computation on data vector 100000", 100000],
        ["Do computation on data vector 1000000", 1000000],
        ["Do computation on data vector 10000000", 10000000],
        ["Do computation on data vector 100000000", 100000000]
    ])
    def testHugeDataTransform(self, test_name, data_size):
        print("\n--Test %s BEGIN--" % test_name)

        group_data = urx.GroupData()
        tmp = np.arange(data_size, dtype=np.float32)
        group_data.raw_data = tmp
        tmp /= 20
        self.assertTrue(np.array_equal(group_data.raw_data, tmp))
        group_data.raw_data[-1]
        ref = group_data.raw_data
        tic = perf_counter()
        ref /= 20
        self.assertTrue(np.array_equal(group_data.raw_data, ref))
        self.assertTrue(np.array_equal(group_data.raw_data, tmp))
        toc = perf_counter() - tic

        print(f"  Dividing np.array by 20 computing time: {toc:.3f}")

        print("--Test %s END--" % test_name)

    @parameterized.expand([
        ["Complex complex64", np.complex64, np.float32],
        ["Complex complex128", np.complex128, np.float64]
    ])
    def testRawDataFloatComplex(self, test_name, np_comp_type, np_type):
        print("\n--Test %s BEGIN--" % test_name)

        group_data = urx.GroupData()
        self.assertEqual(len(group_data.raw_data), 0)
        self.assertEqual(group_data.raw_data.shape, (0,))

        group_data.raw_data = np.array([], dtype=np_comp_type)
        self.assertEqual(len(group_data.raw_data), 0)
        self.assertEqual(group_data.raw_data.size, 0)
        self.assertEqual(group_data.raw_data.shape, (0, 2))

        group_data.raw_data = np.array([np_comp_type()])
        self.assertEqual(len(group_data.raw_data), 1)
        self.assertEqual(group_data.raw_data.size, 2)
        self.assertEqual(group_data.raw_data.shape, (1, 2))

        group_data.raw_data = np_comp_type([1.+2.j, 2.+3.j])
        self.assertEqual(group_data.raw_data.size, 4)
        self.assertEqual(len(group_data.raw_data), 2)
        self.assertEqual(group_data.raw_data.shape, (2, 2))

        group_data.raw_data = np.array([1.+2.j, 2.+3.j], dtype=np_comp_type)
        self.assertEqual(group_data.raw_data.size, 4)
        self.assertEqual(len(group_data.raw_data), 2)
        self.assertEqual(group_data.raw_data.shape, (2, 2))

        ref = group_data.raw_data
        self.assertTrue(np.array_equal(np.array(
            [[1, 2], [2, 3]], dtype=np_type), ref))
        self.assertTrue(np.array_equal(np.array(
            [[1, 2], [2, 3]], dtype=np_type), group_data.raw_data))

        ref[0] = [45, 67]
        self.assertTrue(np.array_equal(np.array(
            [[45, 67], [2, 3]], dtype=np_type), ref))
        self.assertTrue(np.array_equal(np.array(
            [[45, 67], [2, 3]], dtype=np_type), group_data.raw_data))
        self.assertTrue(np.array_equal(
            group_data.raw_data.view(np_comp_type)[:, 0], np_comp_type([45+67j, 2+3j])))

        ref += 1
        self.assertTrue(np.array_equal(ref, np.array(
            [[46, 68], [3, 4]], dtype=np_type)))
        self.assertTrue(np.array_equal(ref, group_data.raw_data))

        ref[:, 0] += 1
        self.assertTrue(np.array_equal(ref, np.array(
            [[47, 68], [4, 4]], dtype=np_type)))
        self.assertTrue(np.array_equal(ref, group_data.raw_data))

        comp_ref = group_data.raw_data.view(np_comp_type)[:, 0]
        comp_ref += 1
        self.assertTrue(np.array_equal(comp_ref, [48+68j, 5+4j]))
        self.assertFalse(np.array_equal(
            group_data.raw_data.view(np_comp_type), comp_ref))

        print("--Test %s END--" % test_name)

    @parameterized.expand([
        ["Complex int16", np.int16],
        ["Complex int32", np.int32],
        ["Complex float32", np.float32],
        ["Complex float64", np.float64]
    ])
    def testRawDataUniversalComplex(self, test_name, np_type):
        print("\n--Test %s BEGIN--" % test_name)

        group_data = urx.GroupData()
        self.assertEqual(len(group_data.raw_data), 0)
        self.assertEqual(group_data.raw_data.shape, (0,))

        group_data.raw_data = np.array([[0, 0]], dtype=np_type)
        self.assertEqual(len(group_data.raw_data), 1)
        self.assertEqual(group_data.raw_data.size, 2)
        self.assertEqual(group_data.raw_data.shape, (1, 2))

        group_data.raw_data = np.array([[np_type(), np_type()]])
        self.assertEqual(len(group_data.raw_data), 1)
        self.assertEqual(group_data.raw_data.size, 2)
        self.assertEqual(group_data.raw_data.shape, (1, 2))

        group_data.raw_data = np_type([[1, 2], [2, 3]])
        self.assertEqual(group_data.raw_data.size, 4)
        self.assertEqual(len(group_data.raw_data), 2)
        self.assertEqual(group_data.raw_data.shape, (2, 2))

        group_data.raw_data = np.array([[1, 2], [2, 3]], dtype=np_type)
        self.assertEqual(group_data.raw_data.size, 4)
        self.assertEqual(len(group_data.raw_data), 2)
        self.assertEqual(group_data.raw_data.shape, (2, 2))

        ref = group_data.raw_data
        self.assertTrue(np.array_equal(np.array(
            [[1, 2], [2, 3]], dtype=np_type), ref))
        self.assertTrue(np.array_equal(np.array(
            [[1, 2], [2, 3]], dtype=np_type), group_data.raw_data))

        ref[0] = [45, 67]
        self.assertTrue(np.array_equal(np.array(
            [[45, 67], [2, 3]], dtype=np_type), ref))
        self.assertTrue(np.array_equal(np.array(
            [[45, 67], [2, 3]], dtype=np_type), group_data.raw_data))

        ref += 1
        self.assertTrue(np.array_equal(ref, np.array(
            [[46, 68], [3, 4]], dtype=np_type)))
        self.assertTrue(np.array_equal(ref, group_data.raw_data))

        ref[:, 0] += 1
        self.assertTrue(np.array_equal(ref, np.array(
            [[47, 68], [4, 4]], dtype=np_type)))
        self.assertTrue(np.array_equal(ref, group_data.raw_data))

        print("--Test %s END--" % test_name)

    @parameterized.expand([
        ["Real int16", np.int16],
        ["Real int32", np.int32],
        ["Real float32", np.float32],
        ["Real float64", np.float64]
    ])
    def testRawDataUniversalComplex(self, test_name, np_type):
        print("\n--Test %s BEGIN--" % test_name)

        group_data = urx.GroupData()
        self.assertEqual(len(group_data.raw_data), 0)
        self.assertEqual(group_data.raw_data.shape, (0,))

        group_data.raw_data = np.array([0], dtype=np_type)
        self.assertEqual(len(group_data.raw_data), 1)
        self.assertEqual(group_data.raw_data.size, 1)
        self.assertEqual(group_data.raw_data.shape, (1,))

        group_data.raw_data = np.array([np_type()])
        self.assertEqual(len(group_data.raw_data), 1)
        self.assertEqual(group_data.raw_data.size, 1)
        self.assertEqual(group_data.raw_data.shape, (1,))

        group_data.raw_data = np_type([1, 2, 2, 3])
        self.assertEqual(group_data.raw_data.size, 4)
        self.assertEqual(len(group_data.raw_data), 4)
        self.assertEqual(group_data.raw_data.shape, (4,))

        group_data.raw_data = np.array([1, 2, 2, 3], dtype=np_type)
        self.assertEqual(group_data.raw_data.size, 4)
        self.assertEqual(len(group_data.raw_data), 4)
        self.assertEqual(group_data.raw_data.shape, (4,))

        ref = group_data.raw_data
        self.assertTrue(np.array_equal(np.array(
            [1, 2, 2, 3], dtype=np_type), ref))
        self.assertTrue(np.array_equal(np.array(
            [1, 2, 2, 3], dtype=np_type), group_data.raw_data))

        ref[0] = 45
        self.assertTrue(np.array_equal(np.array(
            [45, 2, 2, 3], dtype=np_type), ref))
        self.assertTrue(np.array_equal(np.array(
            [45, 2, 2, 3], dtype=np_type), group_data.raw_data))

        ref += 1
        self.assertTrue(np.array_equal(ref, np.array(
            [46, 3, 3, 4], dtype=np_type)))
        self.assertTrue(np.array_equal(ref, group_data.raw_data))

        ref[::2] += 1
        self.assertTrue(np.array_equal(ref, np.array(
            [47, 3, 4, 4], dtype=np_type)))
        self.assertTrue(np.array_equal(ref, group_data.raw_data))

        print("--Test %s END--" % test_name)

    def testGroupData(self):
        test_name = "GroupData binding"
        print("\n--Test %s BEGIN--" % test_name)

        # Check default CTOR
        group_data = urx.GroupData()
        self.assertRaises(
            RuntimeError, lambda group_data: group_data.group, group_data)
        self.assertEqual(group_data.raw_data.size, 0)
        self.assertTrue(np.array_equal(group_data.raw_data,
                        np.empty(0, dtype=np.float32)))
        self.assertEqual(group_data.group_timestamp, urx.DoubleNan())
        self.assertEqual(group_data.sequence_timestamps, urx.VecFloat64())
        self.assertEqual(group_data.event_timestamps, urx.VecVecFloat64())

        # Check copy CTOR and referencing object
        group_data_2 = urx.GroupData(group_data)
        self.assertEqual(group_data, group_data_2)
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
        # Check assignment
        group_data.group_timestamp = urx.DoubleNan(0)
        self.assertEqual(group_data.group_timestamp,
                         group_timestamp_ref)
        self.assertEqual(group_data.group_timestamp, 0)
        self.assertEqual(group_data, group_data_2)

        # Reference is possible for sequence_timestamps (VecFloat64)
        group_data.sequence_timestamps = urx.VecFloat64([1, 2, 3, 4.56])
        group_data_2 = urx.GroupData(group_data)
        self.assertEqual(group_data.sequence_timestamps, [1, 2, 3, 4.56])
        sequence_timestamps_ref = group_data.sequence_timestamps
        sequence_timestamps_ref[0] = 11
        self.assertEqual(group_data.sequence_timestamps,
                         sequence_timestamps_ref)
        self.assertNotEqual(group_data, group_data_2)
        # Check assignment
        group_data.sequence_timestamps = [1, 2, 3, 4.56]
        self.assertEqual(group_data.sequence_timestamps,
                         sequence_timestamps_ref)
        self.assertEqual(group_data.sequence_timestamps, [1, 2, 3, 4.56])
        self.assertEqual(group_data, group_data_2)

        # Reference is possible for event_timestamps (VecVecFloat64)
        group_data.event_timestamps = urx.VecVecFloat64(
            [[1, 2, 3, 4.56], [7.8, 9]])
        group_data_2 = urx.GroupData(group_data)
        self.assertEqual(group_data.event_timestamps,
                         [[1, 2, 3, 4.56], [7.8, 9]])
        event_timestamps_ref = group_data.event_timestamps
        event_timestamps_ref[0] = [11, 12.34]
        self.assertEqual(group_data.event_timestamps,
                         event_timestamps_ref)
        self.assertNotEqual(group_data, group_data_2)
        # Check assignment
        group_data.event_timestamps = [[1, 2, 3, 4.56], [7.8, 9]]
        self.assertEqual(group_data.event_timestamps,
                         event_timestamps_ref)
        self.assertEqual(group_data.event_timestamps,
                         [[1, 2, 3, 4.56], [7.8, 9]])
        self.assertEqual(group_data, group_data_2)

        # group is a pointer and will always be shared
        group = urx.Group()
        # Check assignment
        group_data.group = group
        group_data_2 = urx.GroupData(group_data)
        self.assertEqual(group_data.group, group)
        group_ref = group_data.group
        group_ref.description = "BMode"
        self.assertEqual(group_data.group, group_ref)
        self.assertEqual(group_data.group, group)
        self.assertEqual(group_data, group_data_2)
        del group
        del group_ref
        gc.collect()
        self.assertRaises(
            RuntimeError, lambda group_data: group_data.group, group_data)
        self.assertRaises(
            RuntimeError, lambda group_data: group_data.group, group_data_2)

        print("--Test %s END--" % test_name)


if __name__ == '__main__':
    unittest.main()
