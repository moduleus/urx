import pyurx as urx
import numpy as np
import unittest


class TestBindingsElement(unittest.TestCase):

    def testElement(self):
        testName = "Element binding"
        print("\n--Test %s BEGIN--" % testName)

        v = urx.Vector3D()
        v_2 = urx.Vector3D(1, 2, 3)
        v_3 = urx.Vector3D(4, 5, 6)
        eg = urx.ElementGeometry([v, v_2])

        t = urx.Transform(v_2, v_2)
        ir = urx.ImpulseResponse(42, np.nan, "sec", [3.14, -42])

        elt = urx.Element()
        self.assertEqual(elt.transform, urx.Transform())
        self.assertRaises(
            RuntimeError, lambda elt: elt.element_geometry, elt)
        self.assertRaises(
            RuntimeError, lambda elt: elt.impulse_response, elt)

        elt.transform = t
        self.assertEqual(elt.transform, urx.Transform(v_2, v_2))
        t.rotation = v
        self.assertEqual(elt.transform, urx.Transform(v_2, v_2))
        t_ref = elt.transform
        t_ref.rotation = v
        self.assertEqual(elt.transform, urx.Transform(v, v_2))

        elt.element_geometry = eg
        self.assertEqual(elt.element_geometry, urx.ElementGeometry([v, v_2]))
        eg.perimeter[0] = v_3
        self.assertEqual(elt.element_geometry, urx.ElementGeometry([v_3, v_2]))
        # self.assertEqual(elt.impulse_response, urx.ImpulseResponse())

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
