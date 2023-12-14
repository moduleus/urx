import pyurx as urx
import numpy as np
import unittest


class TestBindingsStdVecOpaqueBinding(unittest.TestCase):

    # def test_vector_custom(self):
    #     v_a = urx.VectorEl()
    #     v_a.append(urx.El(1))
    #     v_a.append(urx.El(2))
    #     assert str(v_a) == "VectorEl[El{1}, El{2}]"

    #     vv_a = urx.VectorVectorEl()
    #     vv_a.append(v_a)
    #     vv_b = vv_a[0]
    #     assert str(vv_b) == "VectorEl[El{1}, El{2}]"

    def testStdVecOpaqueBinding(self):
        testName = "StdVecOpaqueBinding binding"
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

        # v_4 is a reference to first element of C++ vector
        v_4 = vec[0]
        # Modify v_4 affects C++ vector
        v_4.z = 42
        self.assertEqual(vec[0].z, v_4.z)
        self.assertEqual(vec[0], v_4)
        self.assertEqual(vec, urx.VecVector3D([v_4, v_2]))
        self.assertEqual(vec, [v_4, v_2])
        v_5 = urx.Vector3D(v_4)

        vec.append(v_3)
        self.assertEqual(len(vec), 3)
        self.assertEqual(vec[0], v_5)
        self.assertNotEqual(vec[0], v_4)
        self.assertEqual(vec[1], v_2)
        self.assertEqual(vec[2], v_3)
        self.assertEqual(vec, [v_5, v_2, v_3])
        self.assertEqual(vec, urx.VecVector3D([v_5, v_2, v_3]))

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
