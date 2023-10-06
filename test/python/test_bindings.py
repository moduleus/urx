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

        self.assertEqual(group_data.raw_data, uff.VecInt16())
        group_data.raw_data = uff.VecFloat64([1., 2., 3., 4.])
        checkInternalType(self, "float64", group_data)
        group_data.raw_data.append(42.)
        group_data.raw_data = uff.VecFloat64(np.asarray(group_data.raw_data)*2)
        self.assertEqual(group_data.raw_data,
                         uff.VecFloat64([2., 4., 6., 8., 84]))
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

        group_data.raw_data = uff.VecCompInt16(
            np.array([[1, 3], [2, 4], [1, 5]], dtype=np.int16))
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
        group_data.raw_data = uff.VecFloat32(
            np.array(range(data_size), dtype=np.float32))
        ref = np.array(group_data.raw_data, copy=False)
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
        self.assertEqual(type(group_data.raw_data), uffType)

        group_data.raw_data = uffType([1.+2.j, 2.+3.j])
        self.assertEqual(len(group_data.raw_data), 2)
        self.assertEqual(type(group_data.raw_data), uffType)

        group_data.raw_data = uffType(
            np.array([1.+2.j, 2.+3.j], dtype=numpyType))
        self.assertEqual(len(group_data.raw_data), 2)
        self.assertEqual(type(group_data.raw_data), uffType)

        ref = np.array(group_data.raw_data, copy=False)
        for i in range(len(group_data.raw_data)):
            self.assertEqual(group_data.raw_data[i].real, ref[i].real)
            self.assertEqual(group_data.raw_data[i].imag, ref[i].imag)

        ref[0] = 45.+67.j
        for i in range(len(group_data.raw_data)):
            self.assertEqual(group_data.raw_data[i].real, ref[i].real)
            self.assertEqual(group_data.raw_data[i].imag, ref[i].imag)

        ref += 1
        self.assertTrue(np.array_equal(ref, np.array(
            [46.+67.j, 3.+3.j], dtype=numpyType)))
        for i in range(len(group_data.raw_data)):
            self.assertEqual(group_data.raw_data[i].real, ref[i].real)
            self.assertEqual(group_data.raw_data[i].imag, ref[i].imag)

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


if __name__ == '__main__':
    unittest.main()
