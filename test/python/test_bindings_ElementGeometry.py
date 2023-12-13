import pyurx as urx
import numpy as np
import unittest


class TestBindingsElementGeometry(unittest.TestCase):

    def testElementGeometry(self):
        testName = "ElementGeometry binding"
        print("\n--Test %s BEGIN--" % testName)

        eg = urx.ElementGeometry()
        eg_2 = urx.ElementGeometry()

        v = urx.Vector3D()
        v_2 = urx.Vector3D(1, 2, 3)
        v_3 = urx.Vector3D(4, 5, 6)

        self.assertEqual(len(eg.perimeter), 0)
        self.assertEqual(eg, eg_2)

        eg.perimeter = urx.VecVector3D([v, v_2])
        self.assertNotEqual(eg, eg_2)
        eg_2 = urx.ElementGeometry(urx.VecVector3D([v, v_2]))
        self.assertEqual(eg, eg_2)

        eg.perimeter[0] = v_3
        self.assertEqual(eg.perimeter[0], v_3)
        self.assertEqual(eg, urx.ElementGeometry([v_3, v_2]))
        eg.perimeter.append(v_3)
        self.assertEqual(eg, urx.ElementGeometry([v_3, v_2, v_3]))

        eg.perimeter[0].x = 951
        self.assertEqual(eg.perimeter[0], v_3)
        v_3.y = 753
        self.assertEqual(eg.perimeter[0], v_3)

        v.x = 42
        self.assertEqual(eg, eg_2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
