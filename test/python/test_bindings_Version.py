import pyurx as urx
import numpy as np
import unittest


class TestBindingsVersion(unittest.TestCase):

    def testVersion(self):
        testName = "Version binding"
        print("\n--Test %s BEGIN--" % testName)

        v = urx.Version()
        v_2 = urx.Version()

        self.assertEqual(v.major, urx.URX_VERSION_MAJOR)
        self.assertEqual(v.minor, urx.URX_VERSION_MINOR)
        self.assertEqual(v.patch, urx.URX_VERSION_PATCH)
        self.assertEqual(v, v_2)

        v = urx.Version(1, 2, 3)
        self.assertNotEqual(v, v_2)
        v_2 = urx.Version(1, 2, 3)
        self.assertEqual(v, v_2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
