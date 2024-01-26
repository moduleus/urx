import pyurx as urx
import unittest


class TestBindingsTransform(unittest.TestCase):

    def testTransform(self):
        testName = "Transform binding"
        print("\n--Test %s BEGIN--" % testName)

        tf = urx.Transform()

        self.assertEqual(tf.rotation, urx.Vector3D())
        self.assertEqual(tf.translation, urx.Vector3D())
        r = urx.Vector3D(1, 2, -3.14)
        t = urx.Vector3D(4, 5, -6.78)

        tf = urx.Transform(r, t)
        self.assertEqual(tf.rotation, r)
        self.assertEqual(tf.translation, t)

        tf_2 = urx.Transform(tf)
        self.assertEqual(tf, tf_2)

        r_2 = r
        r.x = 42
        self.assertNotEqual(tf.rotation, r)
        self.assertNotEqual(tf.rotation, r_2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
