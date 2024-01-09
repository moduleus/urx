import pyurx as urx
import numpy as np
import unittest
import gc


class TestBindingsElement(unittest.TestCase):

    def testElement(self):
        testName = "Element binding"
        print("\n--Test %s BEGIN--" % testName)

        v = urx.Vector3D()
        v_2 = urx.Vector3D(1, 2, 3)
        v_3 = urx.Vector3D(4, 5, 6)

        # Check default CTOR
        elt = urx.Element()
        self.assertEqual(elt.transform, urx.Transform())
        self.assertRaises(
            RuntimeError, lambda elt: elt.element_geometry, elt)
        self.assertRaises(
            RuntimeError, lambda elt: elt.impulse_response, elt)

        # Check copy CTOR and referencing object
        elt_2 = urx.Element(elt)
        self.assertEqual(elt, elt_2)
        elt_2.transform = urx.Transform(v_2, v_2)
        self.assertNotEqual(elt, elt_2)
        elt_ref = elt
        elt_ref.transform = urx.Transform(v_2, v_2)
        self.assertEqual(elt, elt_ref)

        t = urx.Transform(v_3, v_3)

        # Check CTOR with all parameters
        elt = urx.Element(t, urx.ElementGeometry(), urx.ImpulseResponse())
        self.assertRaises(
            RuntimeError, lambda elt: elt.element_geometry, elt)
        self.assertRaises(
            RuntimeError, lambda elt: elt.impulse_response, elt)
        self.assertEqual(elt.transform, urx.Transform(v_3, v_3))
        t.rotation = v
        self.assertEqual(elt.transform, urx.Transform(v_3, v_3))
        self.assertNotEqual(elt.transform, t)

        t_ref = elt.transform
        t_ref.rotation = v
        self.assertEqual(elt.transform, t_ref)

        eg = urx.ElementGeometry([v, v_2])

        # Reference is possible for element_geometry since it's a weak_ptr
        # Check assignment
        elt.element_geometry = eg
        self.assertEqual(elt.element_geometry, urx.ElementGeometry([v, v_2]))
        eg.perimeter[0] = v_3
        self.assertEqual(elt.element_geometry, urx.ElementGeometry([v_3, v_2]))
        eg_ref = elt.element_geometry
        eg_ref.perimeter[0] = v
        self.assertEqual(elt.element_geometry, eg)
        self.assertEqual(elt.element_geometry, eg_ref)
        del eg
        gc.collect()
        self.assertEqual(elt.element_geometry, eg_ref)
        del eg_ref
        gc.collect()
        # Check the weak_ptr does not reference anymore the deleted object
        self.assertRaises(
            RuntimeError, lambda elt: elt.element_geometry, elt)

        ir = urx.ImpulseResponse(42, np.nan, "sec", [3.14, -42])

        # Reference is possible for element_geometry since it's a weak_ptr
        # Check assignment
        elt.impulse_response = ir
        self.assertEqual(elt.impulse_response, urx.ImpulseResponse(
            42, np.nan, "sec", [3.14, -42]))
        ir.sampling_frequency.value = 123
        self.assertEqual(elt.impulse_response, urx.ImpulseResponse(
            123, np.nan, "sec", [3.14, -42]))
        ir_ref = elt.impulse_response
        ir_ref.sampling_frequency.value = np.nan
        self.assertEqual(elt.impulse_response, ir)
        self.assertEqual(elt.impulse_response, ir_ref)
        del ir
        gc.collect()
        self.assertEqual(elt.impulse_response, ir_ref)
        del ir_ref
        gc.collect()
        # Check the weak_ptr does not reference anymore the deleted object
        self.assertRaises(
            RuntimeError, lambda elt: elt.impulse_response, elt)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
