import pyurx as urx
import numpy as np
import unittest


class TestBindingsStdVecOpaqueBinding(unittest.TestCase):

    def testVecVector3D(self):
        testName = "VecVector3D"
        print("\n--Test %s binding BEGIN--" % testName)
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
        self.assertNotEqual(vec[0], v_ref)
        self.assertEqual(vec[1], v_2)
        self.assertEqual(vec[2], v_3)
        self.assertEqual(vec, [v_5, v_2, v_3])
        self.assertEqual(vec, urx.VecVector3D([v_5, v_2, v_3]))

        print("--Test %s END--" % testName)

    def testVecElementGeometryPtr(self):
        testName = "VecElementGeometryPtr"
        print("\n--Test %s binding BEGIN--" % testName)

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

        # Modify eg_3 DOES affect C++ vector since vec is a pointer vector is a pointer
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

        # elt_ref is not deallocated during the reallocation caused by the append since it is a pointer
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
        testName = "VecImpulseResponsePtr"
        print("\n--Test %s binding BEGIN--" % testName)

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

        # Modify ir_3 DOES affect C++ vector since vec is a pointer vector is a pointer
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
        testName = "VecElement"
        print("\n--Test %s binding BEGIN--" % testName)

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
        self.assertNotEqual(vec[0], elt_ref)
        self.assertEqual(vec[1], elt_2)
        self.assertEqual(vec[2], elt_3)
        self.assertEqual(vec, [elt_5, elt_2, elt_3])
        self.assertEqual(vec, urx.VecElement([elt_5, elt_2, elt_3]))

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
