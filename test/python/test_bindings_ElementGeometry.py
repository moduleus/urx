import pyurx as urx
import numpy as np
import unittest

class TestBindingsElementGeometry(unittest.TestCase):

    def testElementGeometry(self):
        testName = "ElementGeometry binding"
        print("\n--Test %s BEGIN--" % testName)

        eg = urx.ElementGeometry()
        eg2 = urx.ElementGeometry()

        v = urx.Vector3D()
        v2 = urx.Vector3D(1,2,3)

        self.assertEqual(len(eg.perimeter), 0)
        self.assertEqual(eg, eg2)

        eg.perimeter = [v,v2]
        self.assertNotEqual(eg, eg2)
        eg2 = urx.ElementGeometry([v,v2])
        self.assertEqual(eg, v2)

        print("--Test %s END--" % testName)

if __name__ == '__main__':
    unittest.main()
