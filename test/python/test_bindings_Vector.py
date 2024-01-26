import pyurx as urx
import unittest


class TestBindingsVector2D(unittest.TestCase):

    def testVector2D(self):
        testName = "Vector2D binding"
        print("\n--Test %s BEGIN--" % testName)

        # Check default CTOR
        v = urx.Vector2D()
        v_2 = urx.Vector2D(1, 2)
        self.assertEqual(v.x, 0)
        self.assertEqual(v.y, 0)

        # Check copy CTOR and referencing object
        v_2 = urx.Vector2D(v)
        self.assertEqual(v, v_2)
        v_2.x = 42
        self.assertNotEqual(v, v_2)
        v_ref = v
        v_ref.x = 42
        self.assertEqual(v, v_ref)

        # Check CTOR with all parameters
        v = urx.Vector2D(1, 2)
        self.assertEqual(v.x, 1)
        self.assertEqual(v.y, 2)

        print("--Test %s END--" % testName)

    def testVector3D(self):
        testName = "Vector3D binding"
        print("\n--Test %s BEGIN--" % testName)

        # Check default CTOR
        v = urx.Vector3D()
        self.assertEqual(v.x, 0)
        self.assertEqual(v.y, 0)
        self.assertEqual(v.z, 0)

        # Check copy CTOR and referencing object
        v_2 = urx.Vector3D(v)
        self.assertEqual(v, v_2)
        v_2.x = 42
        self.assertNotEqual(v, v_2)
        v_ref = v
        v_ref.x = 42
        self.assertEqual(v, v_ref)

        # Check CTOR with all parameters
        v = urx.Vector3D(1, 2, 3)
        self.assertEqual(v.x, 1)
        self.assertEqual(v.y, 2)
        self.assertEqual(v.z, 3)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
