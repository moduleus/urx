import pyurx as urx
import numpy as np
import unittest


class TestBindingsStdVecOpaqueBinding(unittest.TestCase):

    def testVecFloat64(self):
        testName = "VecFloat64 binding"
        print("\n--Test %s BEGIN--" % testName)

        self.assertEqual(urx.VecFloat64(), [])
        self.assertEqual(urx.VecFloat64([1.23, 2.34]), [1.23, 2.34])
        self.assertTrue(np.array_equal(urx.VecFloat64([1.23, 2.34]), np.array(
            [1.23, 2.34])))

        v = [1.23, 2.34]
        toto = v[0]
        toto += 1
        self.assertEqual(v, [1.23, 2.34])
        v_ref = v
        v_ref[0] += 1
        self.assertEqual(v, [2.23, 2.34])

        print("--Test %s END--" % testName)

    def testVecUInt32(self):
        testName = "VecUInt32 binding"
        print("\n--Test %s BEGIN--" % testName)

        self.assertEqual(urx.VecUInt32(), [])
        self.assertEqual(urx.VecUInt32([1, 2, 3]), [1, 2, 3])
        self.assertTrue(np.array_equal(urx.VecUInt32([1, 2, 3]), np.array(
            [1, 2, 3])))

        v = [1, 2, 3]
        toto = v[0]
        toto += 1
        self.assertEqual(v, [1, 2, 3])
        v_ref = v
        v_ref[0] += 1
        self.assertEqual(v, [2, 2, 3])

        print("--Test %s END--" % testName)

    def testVecVecUInt32(self):
        testName = "VecVecUInt32 binding"
        print("\n--Test %s BEGIN--" % testName)

        self.assertEqual(urx.VecVecUInt32(), [])
        self.assertEqual(urx.VecVecUInt32([[1, 2, 3], [4, 5, 6, 7, 8, 9]]), [
                         [1, 2, 3], [4, 5, 6, 7, 8, 9]])

        v = urx.VecVecUInt32([[1, 2, 3], [4, 5, 6, 7, 8, 9]])
        toto = v[0][0]
        toto += 1
        self.assertEqual(v[0], [1, 2, 3])
        v0_ref = v[0]
        v0_ref[0] += 1
        self.assertEqual(v[0], [2, 2, 3])
        v_ref = v
        v_ref[0][0] += 1
        self.assertEqual(v[0], [3, 2, 3])
        v_ref.append([10, 11])
        self.assertEqual(v[2], [10, 11])

        print("--Test %s END--" % testName)

    def testVecVecFloat64(self):
        testName = "VecVecFloat64 binding"
        print("\n--Test %s BEGIN--" % testName)

        self.assertEqual(urx.VecVecFloat64(), [])
        self.assertEqual(urx.VecVecFloat64([[1, 2, 3], [4, 5, 6, 7, 8, 9]]), [
                         [1, 2, 3], [4, 5, 6, 7, 8, 9]])

        v = urx.VecVecFloat64([[1, 2, 3], [4, 5, 6, 7, 8, 9]])
        toto = v[0][0]
        toto += 1
        self.assertEqual(v[0], [1, 2, 3])
        v0_ref = v[0]
        v0_ref[0] += 1
        self.assertEqual(v[0], [2, 2, 3])
        v_ref = v
        v_ref[0][0] += 1
        self.assertEqual(v[0], [3, 2, 3])
        v_ref.append([10, 11])
        self.assertEqual(v[2], [10, 11])

        print("--Test %s END--" % testName)

    def testVecVector3D(self):
        testName = "VecVector3D binding"
        print("\n--Test %s BEGIN--" % testName)
        v = urx.Vector3D()
        v_2 = urx.Vector3D(1, 2, 3)
        v_3 = urx.Vector3D(4, 5, 6)

        # List
        v_list = [v, v_2]
        self.assertEqual(len(v_list), 2)
        self.assertEqual(v_list, [v, v_2])
        v_list[0] = v_3
        self.assertEqual(v_list[0], v_3)
        self.assertEqual(v_list, [v_3, v_2])
        v_3.x = 42
        self.assertEqual(v_list[0].x, v_3.x)
        self.assertEqual(v_list[0], v_3)
        self.assertEqual(v_list, [v_3, v_2])
        v_list.append(v_3)
        self.assertEqual(len(v_list), 3)
        self.assertEqual(v_list, [v_3, v_2, v_3])

        # np.array
        v_arr = np.array([v, v_2])
        self.assertEqual(len(v_arr), 2)
        self.assertTrue(np.array_equal(v_arr, np.array([v, v_2])))
        v_arr[0] = v_3
        self.assertEqual(v_arr[0], v_3)
        self.assertTrue(np.array_equal(v_arr, np.array([v_3, v_2])))
        v_3.y = 42
        self.assertEqual(v_arr[0].y, v_3.y)
        self.assertEqual(v_arr[0], v_3)
        self.assertTrue(np.array_equal(v_arr, np.array([v_3, v_2])))
        v_arr = np.append(v_arr, v_3)
        self.assertEqual(len(v_arr), 3)
        self.assertTrue(np.array_equal(v_arr, np.array([v_3, v_2, v_3])))

        # VecVector3D
        vec = urx.VecVector3D([v, v_2])
        vec_2 = urx.VecVector3D(np.array([v, v_2]))
        self.assertEqual(len(vec), 2)
        self.assertEqual(len(vec_2), 2)
        self.assertEqual(vec, vec_2)
        vec[0] = v_3
        self.assertEqual(vec[0], v_3)
        self.assertEqual(vec, urx.VecVector3D([v_3, v_2]))

        # Modify v_3 does not affect C++ vector since it has been copied into it
        v_3.z = 42
        self.assertNotEqual(vec[0].z, v_3.z)
        self.assertNotEqual(vec[0], v_3)
        self.assertNotEqual(vec, urx.VecVector3D([v_3, v_2]))

        # v_ref is a reference to first element of C++ vector
        v_ref = vec[0]
        # Modify v_ref affects C++ vector
        v_ref.z = 42
        self.assertEqual(vec[0].z, v_ref.z)
        self.assertEqual(vec[0], v_ref)
        self.assertEqual(vec, urx.VecVector3D([v_ref, v_2]))
        self.assertEqual(vec, [v_ref, v_2])
        v_5 = urx.Vector3D(v_ref)

        # v_ref is deallocated during the reallocation caused by the append
        vec.append(v_3)
        self.assertEqual(len(vec), 3)
        self.assertEqual(vec[0], v_5)
        self.assertEqual(vec[1], v_2)
        self.assertEqual(vec[2], v_3)
        self.assertEqual(vec, [v_5, v_2, v_3])
        self.assertEqual(vec, urx.VecVector3D([v_5, v_2, v_3]))

        print("--Test %s END--" % testName)

    def testVecElementGeometryPtr(self):
        testName = "VecElementGeometryPtr binding"
        print("\n--Test %s BEGIN--" % testName)

        v = urx.Vector3D()
        v_2 = urx.Vector3D(1, 2, 3)
        v_3 = urx.Vector3D(4, 5, 6)

        eg = urx.ElementGeometry([v, v])
        eg_2 = urx.ElementGeometry([v_2, v_2])
        eg_3 = urx.ElementGeometry([v_3, v_3])

        # List
        eg_list = [eg, eg_2]
        # VecElementGeometryPtr
        vec = urx.VecElementGeometryPtr(eg_list)
        vec_2 = urx.VecElementGeometryPtr(np.array(eg_list))
        self.assertEqual(len(vec), 2)
        self.assertEqual(len(vec_2), 2)
        self.assertEqual(vec, vec_2)
        vec[0] = eg_3
        self.assertEqual(vec[0], eg_3)
        self.assertEqual(vec, urx.VecElementGeometryPtr([eg_3, eg_2]))

        # Modify eg_3 DOES affect C++ since the vector holds pointers
        eg_3.perimeter[0] = v
        self.assertEqual(vec[0], eg_3)
        self.assertEqual(vec, urx.VecElementGeometryPtr([eg_3, eg_2]))

        # eg_ref is a reference to first element of C++ vector
        eg_ref = vec[0]
        # Modify eg_ref affects C++ vector
        eg_ref.perimeter[0] = v
        self.assertEqual(vec[0], eg_ref)
        self.assertEqual(vec, urx.VecElementGeometryPtr([eg_ref, eg_2]))
        self.assertEqual(vec, [eg_ref, eg_2])
        eg_5 = urx.ElementGeometry(eg_ref)

        # eg_ref is not deallocated during the reallocation caused by the append since it is a pointer
        vec.append(eg_3)
        self.assertEqual(len(vec), 3)
        self.assertEqual(vec[0], eg_5)
        self.assertEqual(vec[0], eg_ref)
        self.assertEqual(vec[1], eg_2)
        self.assertEqual(vec[2], eg_3)
        self.assertEqual(vec, [eg_5, eg_2, eg_3])
        self.assertEqual(vec, urx.VecElementGeometryPtr([eg_5, eg_2, eg_3]))

        print("--Test %s END--" % testName)

    def testVecImpulseResponcePtr(self):
        testName = "VecImpulseResponsePtr binding"
        print("\n--Test %s BEGIN--" % testName)

        ir = urx.ImpulseResponse()
        ir_2 = urx.ImpulseResponse(
            42, np.nan, "sec", [3.14, -42])
        ir_3 = urx.ImpulseResponse(
            123, 456, "ms", [1, 2, 3])

        # List
        ir_list = [ir, ir_2]
        # VecImpulseResponsePtr
        vec = urx.VecImpulseResponsePtr(ir_list)
        vec_2 = urx.VecImpulseResponsePtr(np.array(ir_list))
        self.assertEqual(len(vec), 2)
        self.assertEqual(len(vec_2), 2)
        self.assertEqual(vec, vec_2)
        vec[0] = ir_3
        self.assertEqual(vec[0], ir_3)
        self.assertEqual(vec, urx.VecImpulseResponsePtr([ir_3, ir_2]))

        # Modify ir_3 DOES affect C++ since the vector holds pointers
        ir_3.time_offset.value = 123.456
        self.assertEqual(vec[0], ir_3)
        self.assertEqual(vec, urx.VecImpulseResponsePtr([ir_3, ir_2]))

        # ir_ref is a reference to first element of C++ vector
        ir_ref = vec[0]
        # Modify ir_ref affects C++ vector
        ir_ref.time_offset.value = 3.14
        self.assertEqual(vec[0], ir_ref)
        self.assertEqual(vec, urx.VecImpulseResponsePtr([ir_ref, ir_2]))
        self.assertEqual(vec, [ir_ref, ir_2])
        ir_5 = urx.ImpulseResponse(ir_ref)

        # ir_ref is not deallocated during the reallocation caused by the append since it is a pointer
        vec.append(ir_3)
        self.assertEqual(len(vec), 3)
        self.assertEqual(vec[0], ir_5)
        self.assertEqual(vec[0], ir_ref)
        self.assertEqual(vec[1], ir_2)
        self.assertEqual(vec[2], ir_3)
        self.assertEqual(vec, [ir_5, ir_2, ir_3])
        self.assertEqual(
            vec, urx.VecImpulseResponsePtr([ir_5, ir_2, ir_3]))

        print("--Test %s END--" % testName)

    def testVecElement(self):
        testName = "VecElement binding"
        print("\n--Test %s BEGIN--" % testName)

        v = urx.Vector3D()
        v_2 = urx.Vector3D(1, 2, 3)
        v_3 = urx.Vector3D(4, 5, 6)

        elt = urx.Element()
        elt_2 = urx.Element()
        elt_2.transform = urx.Transform(v_2, v_2)
        elt_3 = urx.Element()
        elt_3.transform = urx.Transform(v_3, v_3)
        self.assertNotEqual(elt, elt_2)
        self.assertNotEqual(elt, elt_3)
        self.assertNotEqual(elt_2, elt_3)

        # List
        elt_list = [elt, elt_2]
        # VecElement
        vec = urx.VecElement(elt_list)
        vec_2 = urx.VecElement(np.array(elt_list))
        self.assertEqual(len(vec), 2)
        self.assertEqual(len(vec_2), 2)
        self.assertEqual(vec, vec_2)
        vec[0] = elt_3
        self.assertEqual(vec[0], elt_3)
        self.assertEqual(vec, urx.VecElement([elt_3, elt_2]))

        # Modify elt_3 does not affect C++ vector since it has been copied into it
        elt_3.transform = urx.Transform(v_2, v_3)
        self.assertNotEqual(vec[0], elt_3)
        self.assertNotEqual(vec, urx.VecElement([elt_3, elt_2]))

        # elt_ref is a reference to first element of C++ vector
        elt_ref = vec[0]
        # Modify elt_ref affects C++ vector
        elt_ref.transform.rotation = v
        self.assertEqual(vec[0], elt_ref)
        self.assertEqual(vec, urx.VecElement([elt_ref, elt_2]))
        self.assertEqual(vec, [elt_ref, elt_2])
        elt_5 = urx.Element(elt_ref)

        # elt_ref is deallocated during the reallocation caused by the append
        vec.append(elt_3)
        self.assertEqual(len(vec), 3)
        self.assertEqual(vec[0], elt_5)
        self.assertEqual(vec[1], elt_2)
        self.assertEqual(vec[2], elt_3)
        self.assertEqual(vec, [elt_5, elt_2, elt_3])
        self.assertEqual(vec, urx.VecElement([elt_5, elt_2, elt_3]))

        print("--Test %s END--" % testName)

    def testVecEvent(self):
        testName = "VecEvent binding"
        print("\n--Test %s BEGIN--" % testName)

        rs = urx.ReceiveSetup(urx.Probe(), urx.Transform(),
                              1, 2, [[3]], [4], 5, 6, 7)
        ts = urx.TransmitSetup(urx.Probe(), urx.Wave(), urx.Transform(), 42)

        evt = urx.Event()
        evt_2 = urx.Event(ts, rs)
        evt_3 = urx.Event(evt)
        evt_3.transmit_setup = ts
        self.assertNotEqual(evt, evt_2)
        self.assertNotEqual(evt, evt_3)
        self.assertNotEqual(evt_2, evt_3)

        # List
        evt_list = [evt, evt_2]
        # VecEvent
        vec = urx.VecEvent(evt_list)
        vec_2 = urx.VecEvent(np.array(evt_list))
        self.assertEqual(len(vec), 2)
        self.assertEqual(len(vec_2), 2)
        self.assertEqual(vec, vec_2)
        vec[0] = evt_3
        self.assertEqual(vec[0], evt_3)
        self.assertEqual(vec, urx.VecEvent([evt_3, evt_2]))

        # Modify evt_3 does not affect C++ vector since it has been copied into it
        evt_3.transmit_setup.time_offset.value = 123
        self.assertNotEqual(vec[0], evt_3)
        self.assertNotEqual(vec, urx.VecEvent([evt_3, evt_2]))

        # evt_ref is a reference to first element of C++ vector
        evt_ref = vec[0]
        # Modify evt_ref affects C++ vector
        evt_ref.transmit_setup.time_offset.value = 123
        self.assertEqual(vec[0], evt_ref)
        self.assertEqual(vec, urx.VecEvent([evt_ref, evt_2]))
        self.assertEqual(vec, [evt_ref, evt_2])
        evt_5 = urx.Event(evt_ref)

        # evt_ref is deallocated during the reallocation caused by the append
        vec.append(evt_3)
        self.assertEqual(len(vec), 3)
        self.assertEqual(vec[0], evt_5)
        self.assertEqual(vec[1], evt_2)
        self.assertEqual(vec[2], evt_3)
        self.assertEqual(vec, [evt_5, evt_2, evt_3])
        self.assertEqual(vec, urx.VecEvent([evt_5, evt_2, evt_3]))

        print("--Test %s END--" % testName)

    def testVecExcitationPtr(self):
        testName = "VecExcitationPtr binding"
        print("\n--Test %s BEGIN--" % testName)

        ex = urx.Excitation("linear", 42, urx.DoubleNan(np.nan), [3.14, -42])
        ex_2 = urx.Excitation("linear", 123, 456, [789])
        ex_3 = urx.Excitation("linear", 987, 654, [321])

        # List
        ex_list = [ex, ex_2]
        # VecElement
        vec = urx.VecExcitationPtr(ex_list)
        vec_2 = urx.VecExcitationPtr(np.array(ex_list))
        self.assertEqual(len(vec), 2)
        self.assertEqual(len(vec_2), 2)
        self.assertEqual(vec, vec_2)
        vec[0] = ex_3
        self.assertEqual(vec[0], ex_3)
        self.assertEqual(vec, urx.VecExcitationPtr([ex_3, ex_2]))

        # Modify ex_3 DOES affect C++ vector since the vector holds pointers
        ex_3.sampling_frequency.value = 456.123
        self.assertEqual(vec[0], ex_3)
        self.assertEqual(vec, urx.VecExcitationPtr([ex_3, ex_2]))

        # ex_ref is a reference to first element of C++ vector
        ex_ref = vec[0]
        # Modify ex_ref affects C++ vector
        ex_ref.pulse_shape = "diverging"
        self.assertEqual(vec[0], ex_ref)
        self.assertEqual(vec, urx.VecExcitationPtr([ex_ref, ex_2]))
        self.assertEqual(vec, [ex_ref, ex_2])
        ex_5 = urx.Excitation(ex_ref)

        # ex_ref is deallocated during the reallocation caused by the append
        vec.append(ex_3)
        self.assertEqual(len(vec), 3)
        self.assertEqual(vec[0], ex_5)
        self.assertEqual(vec[0], ex_ref)
        self.assertEqual(vec[1], ex_2)
        self.assertEqual(vec[2], ex_3)
        self.assertEqual(vec, [ex_5, ex_2, ex_3])
        self.assertEqual(vec, urx.VecExcitationPtr([ex_5, ex_2, ex_3]))

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
