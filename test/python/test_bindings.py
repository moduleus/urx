import pyusff as uff
import numpy as np
from time import perf_counter

import unittest
from parameterized import parameterized


class TestBindings(unittest.TestCase):

    def testVariantVectorInternalType(self):
        testName = "Variant Vector Internal Type"
        print("\n--Test %s BEGIN--" % testName)

        group_data = uff.GroupData()
        mapTypeToCheckFunc = {"int16": uff.checkInt16Type, "int32": uff.checkInt32Type,
                              "float32": uff.checkFloat32Type, "float64": uff.checkFloat64Type,
                              "complexInt16": uff.checkComplexInt16Type, "complexInt32": uff.checkComplexInt32Type,
                              "complexFloat32": uff.checkComplexFloat32Type, "complexFloat64": uff.checkComplexFloat64Type}

        def checkInternalType(self, type, group_data):
            for item in mapTypeToCheckFunc:
                if item == type:
                    self.assertTrue(mapTypeToCheckFunc[item](group_data))
                else:
                    self.assertFalse(mapTypeToCheckFunc[item](group_data))

        self.assertTrue(np.array_equal(group_data.raw_data, uff.VecInt16()))
        group_data.raw_data = uff.VecFloat64([1., 2., 3., 4.])
        checkInternalType(self, "float64", group_data)
        group_data.raw_data = np.append(group_data.raw_data, 42.)
        group_data.raw_data *= 2
        self.assertTrue(np.array_equal(group_data.raw_data,
                         np.array([2., 4., 6., 8., 84],dtype=np.float64)))
        checkInternalType(self, "float64", group_data)

        group_data.raw_data = uff.VecInt16(
            np.array([1, 2, -1], dtype=np.int16))
        checkInternalType(self, "int16", group_data)

        group_data.raw_data = uff.VecInt32(
            np.array([1, 2, -1], dtype=np.int32))
        checkInternalType(self, "int32", group_data)

        group_data.raw_data = uff.VecFloat32(
            np.array([1, 2, -1], dtype=np.float32))
        checkInternalType(self, "float32", group_data)

        group_data.raw_data = uff.VecFloat64(
            np.array([1, 2, -1], dtype=np.float64))
        checkInternalType(self, "float64", group_data)

        group_data.raw_data = np.array([[1, 3], [2, 4], [1, 5]], dtype=np.int16)
        checkInternalType(self, "complexInt16", group_data)

        group_data.raw_data = uff.VecCompInt32(
            np.array([[1, 3], [2, 4], [1, 5]], dtype=np.int32))
        checkInternalType(self, "complexInt32", group_data)

        group_data.raw_data = uff.VecCompFloat32(
            np.array([1+3j, 2-4j, -1+5j], dtype=np.complex64))
        checkInternalType(self, "complexFloat32", group_data)

        group_data.raw_data = uff.VecCompFloat64(
            np.array([1+3j, 2-4j, -1+5j], dtype=np.complex128))
        checkInternalType(self, "complexFloat64", group_data)

        print("--Test %s END--" % testName)

    def testHugeDataTransform(self):
        testName = "Do computation on huge data vector"
        print("\n--Test %s BEGIN--" % testName)

        data_size = 100000000
        group_data = uff.GroupData()
        group_data.raw_data = np.array(range(data_size), dtype=np.float32)
        ref = group_data.raw_data
        tic = perf_counter()
        ref /= 20
        toc = perf_counter() - tic

        print(f"  Dividing 400MB np.array by 20 computing time: {toc:.3f}")

        print("--Test %s END--" % testName)

    @parameterized.expand([
        ["Variant Vector Float 32 Complex", uff.VecCompFloat32, np.complex64],
        ["Variant Vector Float 64 Complex", uff.VecCompFloat64, np.complex128]
    ])
    def testVariantVectorFloatComplex(self, testName, uffType, numpyType):
        print("\n--Test %s BEGIN--" % testName)

        group_data = uff.GroupData()
        self.assertEqual(len(group_data.raw_data), 0)

        group_data = uff.GroupData()
        group_data.raw_data = uffType()
        self.assertEqual(len(group_data.raw_data), 0)

        group_data.raw_data = uffType([1.+2.j, 2.+3.j])
        self.assertEqual(len(group_data.raw_data), 2)

        group_data.raw_data = numpyType([1.+2.j, 2.+3.j])
        self.assertEqual(len(group_data.raw_data), 2)

        group_data.raw_data = uffType(
            np.array([1.+2.j, 2.+3.j], dtype=numpyType))
        self.assertEqual(len(group_data.raw_data), 2)

        group_data.raw_data = np.array([1.+2.j, 2.+3.j], dtype=numpyType)
        self.assertEqual(len(group_data.raw_data), 2)

        ref = group_data.raw_data.view(numpyType)[:,0]
        self.assertTrue(np.array_equal(np.array(
            [1.+2.j, 2.+3.j], dtype=numpyType), ref))
        self.assertTrue(np.array_equal(np.array(
            [[1,2], [2,3]], dtype=numpyType), group_data.raw_data))

        ref[0] = 45.+67j
        self.assertTrue(np.array_equal(group_data.raw_data.view(numpyType)[:,0], ref))

        ref += 1
        self.assertTrue(np.array_equal(ref, np.array(
            [46.+67.j, 3.+3.j], dtype=numpyType)))
        self.assertTrue(np.array_equal(group_data.raw_data.view(numpyType)[:,0], ref))

        print("--Test %s END--" % testName)

    @parameterized.expand([
        ["Variant Vector Int 16 Complex", uff.VecCompInt16, np.int16, "complexInt16"],
        ["Variant Vector Int 32 Complex", uff.VecCompInt32, np.int32, "complexInt32"]
    ])
    def testVariantVectorIntComplex(self, testName, uffType, numpyType, internalType):
        print("\n--Test %s BEGIN--" % testName)

        group_data = uff.GroupData()
        self.assertEqual(len(group_data.raw_data), 0)

        group_data = uff.GroupData()
        mapTypeToCheckFunc = {"int16": uff.checkInt16Type, "int32": uff.checkInt32Type,
                              "float32": uff.checkFloat32Type, "float64": uff.checkFloat64Type,
                              "complexInt16": uff.checkComplexInt16Type, "complexInt32": uff.checkComplexInt32Type,
                              "complexFloat32": uff.checkComplexFloat32Type, "complexFloat64": uff.checkComplexFloat64Type}

        def checkInternalType(self, type, group_data):
            for item in mapTypeToCheckFunc:
                if item == type:
                    self.assertTrue(mapTypeToCheckFunc[item](group_data))
                else:
                    self.assertFalse(mapTypeToCheckFunc[item](group_data))

        arr = np.array([[1, 3], [2, 4], [1, 5]], dtype=numpyType)
        group_data.raw_data = uffType(
            np.array([[1, 3], [2, 4], [1, 5]], dtype=np.int16))
        self.assertTrue(np.array_equal(
            arr, np.array(group_data.raw_data, copy=False)))
        checkInternalType(self, internalType, group_data)

        group_data.raw_data = uffType(
            np.array([[1, 3], [2, 4], [1, 5]], dtype=np.int32))
        self.assertTrue(np.array_equal(
            arr, np.array(group_data.raw_data, copy=False)))
        checkInternalType(self, internalType, group_data)

        group_data.raw_data = uffType(
            np.array([1.9+3.9j, 2.9+4.9j, 1.9+5.9j], dtype=np.complex64))
        self.assertTrue(np.array_equal(
            arr, np.array(group_data.raw_data, copy=False)))
        checkInternalType(self, internalType, group_data)

        group_data.raw_data = uffType(
            np.array([1.9+3.9j, 2.9+4.9j, 1.9+5.9j], dtype=np.complex128))
        self.assertTrue(np.array_equal(
            arr, np.array(group_data.raw_data, copy=False)))
        checkInternalType(self, internalType, group_data)

        print("--Test %s END--" % testName)

    def testGroupData_Group(self):
        testName = "GroupData & Group binding"
        print("\n--Test %s BEGIN--" % testName)

        group_data = uff.GroupData()
        self.assertTrue(np.isnan(group_data.group_timestamp))
        group_data.group_timestamp = np.float64("nan")

        group_data.sequence_timestamps = uff.VecFloat64([1, 2, 3, 4.56])
        self.assertTrue(np.array_equal(group_data.sequence_timestamps, np.array([1, 2, 3, 4.56]))
                        )
        group_data.event_timestamps = [uff.VecFloat64(
            [1, 2, 3, 4.56]), uff.VecFloat64([7.8, 9])]
        self.assertTrue(
            group_data.event_timestamps == [uff.VecFloat64(
                [1, 2, 3, 4.56]), uff.VecFloat64([7.8, 9])])

        group = uff.Group()
        self.assertEqual(group.description, "")
        self.assertEqual(group.sampling_type, uff.SamplingType.UNDEFINED)
        self.assertEqual(group.data_type, uff.DataType.UNDEFINED)

        self.assertRaises(RuntimeError, lambda group_data: group_data.group, group_data)
        group_data.group = group
        self.assertEqual(group_data.group, group)
        group_data.group.description = "Hello world"
        group_data.group.data_type = uff.DataType.FLOAT
        group_data.group.sampling_type = uff.SamplingType.IQ
        self.assertEqual(group_data.group, group)
        import gc
        del group
        gc.collect()
        self.assertRaises(RuntimeError, lambda group_data: group_data.group, group_data)

        print("--Test %s END--" % testName)

    def testAcq_GroupData_Group(self):
        testName = "Acquisition & Groups & GroupData binding"
        print("\n--Test %s BEGIN--" % testName)

        acq = uff.Acquisition()
        group_data = uff.GroupData()
        group = uff.Group()
        acq.groups.append(group)
        group_data.group = group

        self.assertEqual(len(acq.groups), 1)
        self.assertEqual(acq.groups[0].description, "")
        self.assertEqual(acq.groups[0].sampling_type, uff.SamplingType.UNDEFINED)
        self.assertEqual(acq.groups[0].data_type, uff.DataType.UNDEFINED)

        acq.groups[0].description = "Hello World"
        self.assertEqual(group.description, "Hello World")

        acq.groups.append(group)
        self.assertEqual(len(acq.groups), 2)
        acq.groups.append(uff.Group())
        self.assertEqual(len(acq.groups), 3)
        acq.groups[2].sampling_type = uff.SamplingType.IQ

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
        self.assertRaises(RuntimeError, lambda group_data: group_data.group, group_data)
        self.assertEqual(acq.groups[0].sampling_type, uff.SamplingType.IQ)

        print("--Test %s END--" % testName)

    def testDataset(self):
        testName = "Dataset binding"
        print("\n--Test %s BEGIN--" % testName)

        dataset = uff.Dataset()
        acq = uff.Acquisition()
        version = uff.Version()

        self.assertEqual(dataset.acquisition, acq)
        dataset.acquisition.timestamp = 42
        acq.timestamp = 42
        self.assertEqual(dataset.acquisition, acq)

        self.assertEqual(dataset.version, version)
        dataset.version.major = 42
        self.assertNotEqual(dataset.version, version)
        version.major = 42
        self.assertEqual(dataset.version, version)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
