import pyurx as urx
import numpy as np
import unittest


class TestBindingsStdVecOpaqueBinding(unittest.TestCase):

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
        self.assertEqual(v_arr, np.array([v, v_2]))
        v_arr[0] = v_3
        self.assertEqual(v_arr[0], v_3)
        self.assertEqual(v_arr, np.array([v_3, v_2]))
        v_3.x = 42
        self.assertEqual(v_arr[0].x, v_3.x)
        self.assertEqual(v_arr[0], v_3)
        self.assertEqual(v_arr, np.array([v_3, v_2]))
        v_arr.append(v_3)
        self.assertEqual(len(v_arr), 3)
        self.assertEqual(v_arr, np.array([v_3, v_2, v_3]))

        # VecVector3D
        vec = urx.VecVector3D([v, v_2])

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
