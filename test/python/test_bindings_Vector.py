import pyurx as urx
import numpy as np
import unittest


class TestBindingsVector2D(unittest.TestCase):

    def testVector2D(self):
        testName = "Vector2D binding"
        print("\n--Test %s BEGIN--" % testName)

        v = urx.Vector2D()
        v2 = urx.Vector2D()

        self.assertEqual(v.x, 0)
        self.assertEqual(v.y, 0)
        self.assertEqual(v, v2)

        v = urx.Vector2D(1, 2)
        self.assertNotEqual(v, v2)
        v2 = urx.Vector2D(1, 2)
        self.assertEqual(v, v2)

        print("--Test %s END--" % testName)

    def testVector3D(self):
        testName = "Vector3D binding"
        print("\n--Test %s BEGIN--" % testName)

        v = urx.Vector3D()
        v2 = urx.Vector3D()

        self.assertEqual(v.x, 0)
        self.assertEqual(v.y, 0)
        self.assertEqual(v.z, 0)
        self.assertEqual(v, v2)

        v = urx.Vector3D(1, 2, 3)
        self.assertNotEqual(v, v2)
        v2 = urx.Vector3D(1, 2, 3)
        self.assertEqual(v, v2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
