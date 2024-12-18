import numpy as np
from time import perf_counter
import gc
import ultrasound_rawdata_exchange as urx


def test_group_data_huge_transform(
    self,
    test_name,
    data_size,
    group_data_constructor,
):
    print("\n--Test %s BEGIN-" % test_name)

    group_data = group_data_constructor()
    tmp = np.arange(data_size, dtype=np.float32)
    group_data.raw_data = urx.RawData(tmp)
    tmp /= 20
    self.assertTrue(np.allclose(group_data.raw_data, tmp))
    group_data.raw_data[-1]
    ref = group_data.raw_data
    tic = perf_counter()
    ref /= 20
    self.assertTrue(np.allclose(group_data.raw_data, ref))
    self.assertTrue(np.allclose(group_data.raw_data, tmp))
    toc = perf_counter() - tic

    print(f"  Dividing np.array by 20 computing time: {toc:.3f}")

    print("--Test %s END--" % test_name)


def foo(a):
    print(a)


def test_raw_data_float_complex(
    self,
    test_name,
    np_comp_type,
    np_type,
    group_data_constructor,
):
    print("\n--Test %s BEGIN--" % test_name)

    group_data = group_data_constructor()
    # # self.assertEqual(len(group_data.raw_data), 0)
    # self.assertEqual(group_data.raw_data.shape, (0,))

    # group_data.raw_data = np.array([], dtype=np_comp_type)
    # # self.assertEqual(len(group_data.raw_data), 0)
    # self.assertEqual(group_data.raw_data.size, 0)
    # self.assertEqual(group_data.raw_data.shape, (0, 2))

    # group_data.raw_data = np.array([np_comp_type()])
    # # self.assertEqual(len(group_data.raw_data), 1)
    # self.assertEqual(group_data.raw_data.size, 2)
    # self.assertEqual(group_data.raw_data.shape, (1, 2))

    # group_data.raw_data = np_comp_type([1.0 + 2.0j, 2.0 + 3.0j])
    # self.assertEqual(group_data.raw_data.size, 4)
    # # self.assertEqual(len(group_data.raw_data), 2)
    # self.assertEqual(group_data.raw_data.shape, (2, 2))

    group_data.raw_data = urx.RawData(np.array([1.0 + 2.0j, 2.0 + 3.0j], dtype=np_comp_type))
    # self.assertEqual(group_data.raw_data.size, 4)
    # self.assertEqual(len(group_data.raw_data), 2)
    # self.assertEqual(group_data.raw_data.shape, (2, 2))

    ref = np.asarray(group_data.raw_data)
    print(ref)
    print(np.asarray(group_data.raw_data))
    ones = np.ones(np.shape(ref), dtype=np_type)
    print(np.asarray(group_data.raw_data))
    print(ref)
    print(np.asarray(group_data.raw_data))
    print(ones)
    print(np.asarray(group_data.raw_data))
    self.assertTrue(np.allclose(np.array([[1, 2], [2, 3]], dtype=np_type), ref))
    # self.assertTrue(np.allclose(np.array([[1, 2], [2, 3]], dtype=np_type), group_data.raw_data))
    np.allclose(ref, ones)
    print(ref)
    print(ones)
    np.allclose(ref, ones)
    print(ref)
    print(ones)
    np.allclose(ref, ones)
    print(ref)
    print(ones)
    np.allclose(ref, ones)
    print(ref)
    print(ones)

    ref[0] = [45, 67]
    self.assertTrue(np.allclose(np.array([[45, 67], [2, 3]], dtype=np_type), ref))
    self.assertTrue(np.allclose(np.array([[45, 67], [2, 3]], dtype=np_type), group_data.raw_data))
    self.assertTrue(
        np.allclose(
            group_data.raw_data.view(np_comp_type)[:, 0],
            np_comp_type([45 + 67j, 2 + 3j]),
        )
    )

    print(ref)
    print("ref += 1")
    ref += 1
    print("1", ref)
    print("1b", ref)
    gc.collect()
    self.assertTrue(np.allclose(ref, np.array([[46, 68], [3, 4]], dtype=np_type)))
    print("2", ref)
    print("2a", group_data.raw_data)
    gc.collect()
    print(ref, group_data.raw_data)
    print(ref, group_data.raw_data)
    gc.collect()
    self.assertTrue(np.array_equal(group_data.raw_data, ref))

    gc.collect()
    print("3", ref)
    print("3a", group_data.raw_data)
    gc.collect()
    print("3b", ref)
    print("3c", group_data.raw_data)
    gc.collect()
    print("ref[:, 0] += 1")
    ref[:, 0] += 1
    print(ref)
    self.assertTrue(np.allclose(ref, np.array([[47, 68], [4, 4]], dtype=np_type)))
    self.assertTrue(np.allclose(ref, group_data.raw_data))

    comp_ref = group_data.raw_data.view(np_comp_type)[:, 0]
    comp_ref += 1
    self.assertTrue(np.allclose(comp_ref, [48 + 68j, 5 + 4j]))
    self.assertFalse(np.allclose(group_data.raw_data.view(np_comp_type), comp_ref))

    print("--Test %s END--" % test_name)


def test_raw_data_universal_complex(
    self,
    test_name,
    np_type,
    group_data_constructor,
):
    print("\n--Test %s BEGIN--" % test_name)

    group_data = group_data_constructor()
    # self.assertEqual(len(group_data.raw_data), 0)
    self.assertEqual(group_data.raw_data.shape, (0,))

    group_data.raw_data = np.array([[0, 0]], dtype=np_type)
    # self.assertEqual(len(group_data.raw_data), 1)
    self.assertEqual(group_data.raw_data.size, 2)
    self.assertEqual(group_data.raw_data.shape, (1, 2))

    group_data.raw_data = np.array([[np_type(), np_type()]])
    # self.assertEqual(len(group_data.raw_data), 1)
    self.assertEqual(group_data.raw_data.size, 2)
    self.assertEqual(group_data.raw_data.shape, (1, 2))

    group_data.raw_data = np_type([[1, 2], [2, 3]])
    self.assertEqual(group_data.raw_data.size, 4)
    # self.assertEqual(len(group_data.raw_data), 2)
    self.assertEqual(group_data.raw_data.shape, (2, 2))

    group_data.raw_data = np.array([[1, 2], [2, 3]], dtype=np_type)
    self.assertEqual(group_data.raw_data.size, 4)
    # self.assertEqual(len(group_data.raw_data), 2)
    self.assertEqual(group_data.raw_data.shape, (2, 2))

    ref = group_data.raw_data
    self.assertTrue(np.allclose(np.array([[1, 2], [2, 3]], dtype=np_type), ref))
    self.assertTrue(np.allclose(np.array([[1, 2], [2, 3]], dtype=np_type), group_data.raw_data))

    ref[0] = [45, 67]
    self.assertTrue(np.allclose(np.array([[45, 67], [2, 3]], dtype=np_type), ref))
    self.assertTrue(np.allclose(np.array([[45, 67], [2, 3]], dtype=np_type), group_data.raw_data))

    ref += 1
    self.assertTrue(np.allclose(ref, np.array([[46, 68], [3, 4]], dtype=np_type)))
    self.assertTrue(np.allclose(ref, group_data.raw_data))

    ref[:, 0] += 1
    self.assertTrue(np.allclose(ref, np.array([[47, 68], [4, 4]], dtype=np_type)))
    self.assertTrue(np.allclose(ref, group_data.raw_data))

    print("--Test %s END--" % test_name)


def test_raw_data_universal_real(
    self,
    test_name,
    np_type,
    group_data_constructor,
):
    print("\n--Test %s BEGIN--" % test_name)

    group_data = group_data_constructor()
    # self.assertEqual(len(group_data.raw_data), 0)
    self.assertEqual(group_data.raw_data.shape, (0,))

    group_data.raw_data = np.array([0], dtype=np_type)
    # self.assertEqual(len(group_data.raw_data), 1)
    self.assertEqual(group_data.raw_data.size, 1)
    self.assertEqual(group_data.raw_data.shape, (1,))

    group_data.raw_data = np.array([np_type()])
    # self.assertEqual(len(group_data.raw_data), 1)
    self.assertEqual(group_data.raw_data.size, 1)
    self.assertEqual(group_data.raw_data.shape, (1,))

    group_data.raw_data = np_type([1, 2, 2, 3])
    self.assertEqual(group_data.raw_data.size, 4)
    # self.assertEqual(len(group_data.raw_data), 4)
    self.assertEqual(group_data.raw_data.shape, (4,))

    group_data.raw_data = np.array([1, 2, 2, 3], dtype=np_type)
    self.assertEqual(group_data.raw_data.size, 4)
    # self.assertEqual(len(group_data.raw_data), 4)
    self.assertEqual(group_data.raw_data.shape, (4,))

    ref = group_data.raw_data
    self.assertTrue(np.allclose(np.array([1, 2, 2, 3], dtype=np_type), ref))
    self.assertTrue(np.allclose(np.array([1, 2, 2, 3], dtype=np_type), group_data.raw_data))

    ref[0] = 45
    self.assertTrue(np.allclose(np.array([45, 2, 2, 3], dtype=np_type), ref))
    self.assertTrue(np.allclose(np.array([45, 2, 2, 3], dtype=np_type), group_data.raw_data))

    ref += 1
    self.assertTrue(np.allclose(ref, np.array([46, 3, 3, 4], dtype=np_type)))
    self.assertTrue(np.allclose(ref, group_data.raw_data))

    ref[::2] += 1
    self.assertTrue(np.allclose(ref, np.array([47, 3, 4, 4], dtype=np_type)))
    self.assertTrue(np.allclose(ref, group_data.raw_data))

    print("--Test %s END--" % test_name)


def test_group_data(
    self,
    group_data_constructor,
    group_data_copy,
    double_nan_constructor,
    double_nan_args,
    vec_float64_constructor,
    vec_float64_args,
    vec_vec_float64_constructor,
    vec_vec_float64_args,
    group_constructor,
):
    test_name = "GroupData binding"
    print("\n--Test %s BEGIN--" % test_name)

    # Check default CTOR
    group_data = group_data_constructor()
    self.assertEqual(group_data.group, None)
    self.assertEqual(group_data.raw_data.size, 0)
    self.assertTrue(np.allclose(group_data.raw_data, np.empty(0, dtype=np.float32)))
    self.assertEqual(group_data.group_timestamp, double_nan_constructor())
    self.assertEqual(group_data.sequence_timestamps, vec_float64_constructor())
    self.assertEqual(group_data.event_timestamps, vec_vec_float64_constructor())

    # Check copy CTOR and referencing object
    group_data_2 = group_data_copy(group_data)
    self.assertEqual(group_data, group_data_2)
    group_data_2.group_timestamp = double_nan_args(0)
    self.assertNotEqual(group_data, group_data_2)
    group_data_ref = group_data
    group_data_ref.group_timestamp = double_nan_args(0)
    self.assertEqual(group_data, group_data_ref)
    group_data_2 = group_data_copy(group_data)

    # No CTOR with parameters to test

    # Reference is possible for group_timestamp (DoubleNan)
    self.assertEqual(group_data.group_timestamp, 0)
    group_timestamp_ref = group_data.group_timestamp
    group_timestamp_ref.value = 42
    self.assertEqual(group_data.group_timestamp, group_timestamp_ref)
    self.assertNotEqual(group_data, group_data_2)
    # Check assignment
    group_data.group_timestamp = double_nan_args(0)
    self.assertEqual(group_data.group_timestamp, group_timestamp_ref)
    self.assertEqual(group_data.group_timestamp, 0)
    self.assertEqual(group_data, group_data_2)

    # Reference is possible for sequence_timestamps (VecFloat64)
    group_data.sequence_timestamps = vec_float64_args([1, 2, 3, 4.56])
    group_data_2 = group_data_copy(group_data)
    self.assertEqual(group_data.sequence_timestamps, [1, 2, 3, 4.56])
    sequence_timestamps_ref = group_data.sequence_timestamps
    sequence_timestamps_ref[0] = 11
    self.assertEqual(group_data.sequence_timestamps, sequence_timestamps_ref)
    self.assertNotEqual(group_data, group_data_2)
    # Check assignment
    group_data.sequence_timestamps = [1, 2, 3, 4.56]
    self.assertEqual(group_data.sequence_timestamps, sequence_timestamps_ref)
    self.assertEqual(group_data.sequence_timestamps, [1, 2, 3, 4.56])
    self.assertEqual(group_data, group_data_2)

    # Reference is possible for event_timestamps (VecVecFloat64)
    group_data.event_timestamps = vec_vec_float64_args([[1, 2, 3, 4.56], [7.8, 9]])
    group_data_2 = group_data_copy(group_data)
    self.assertEqual(group_data.event_timestamps, [[1, 2, 3, 4.56], [7.8, 9]])
    event_timestamps_ref = group_data.event_timestamps
    event_timestamps_ref[0] = [11, 12.34]
    self.assertEqual(group_data.event_timestamps, event_timestamps_ref)
    self.assertNotEqual(group_data, group_data_2)
    # Check assignment
    group_data.event_timestamps = [[1, 2, 3, 4.56], [7.8, 9]]
    self.assertEqual(group_data.event_timestamps, event_timestamps_ref)
    self.assertEqual(group_data.event_timestamps, [[1, 2, 3, 4.56], [7.8, 9]])
    self.assertEqual(group_data, group_data_2)

    # group is a pointer and will always be shared
    group = group_constructor()
    # Check assignment
    group_data.group = group
    group_data_2 = group_data_copy(group_data)
    self.assertEqual(group_data.group, group)
    group_ref = group_data.group
    group_ref.description = "BMode"
    self.assertEqual(group_data.group, group_ref)
    self.assertEqual(group_data.group, group)
    self.assertEqual(group_data, group_data_2)
    del group
    del group_ref
    gc.collect()
    self.assertEqual(group_data.group, None)
    self.assertEqual(group_data_2.group, None)

    print("--Test %s END--" % test_name)
