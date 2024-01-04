import pyurx as urx
import numpy as np
import unittest
import gc


class TestBindingsProbe(unittest.TestCase):

    def testProbe(self):
        testName = "Probe binding"
        print("\n--Test %s BEGIN--" % testName)

        v = urx.Vector3D()
        v_2 = urx.Vector3D(1, 2, 3)
        v_3 = urx.Vector3D(4, 5, 6)

        # Check default CTOR
        p = urx.Probe()
        self.assertEqual(p.description, "")
        self.assertEqual(p.type, urx.ProbeType.UNDEFINED)
        self.assertEqual(p.transform, urx.Transform())
        self.assertEqual(p.element_geometries, urx.VecElementGeometryPtr())
        self.assertEqual(p.impulse_responses, urx.VecImpulseResponsePtr())
        self.assertEqual(p.elements, urx.VecElement())

        t = urx.Transform()
        t_2 = urx.Transform(v_2, v_2)

        # Check copy CTOR and referencing object
        p_2 = urx.Probe(p)
        self.assertEqual(p, p_2)
        p_2.transform = t_2
        self.assertNotEqual(p, p_2)
        p_ref = p
        p_ref.transform = t_2
        self.assertEqual(p, p_ref)

        eg = urx.ElementGeometry([v, v])
        eg_2 = urx.ElementGeometry([v_2, v_2])
        eg_3 = urx.ElementGeometry([v_3, v_3])

        ir = urx.ImpulseResponse()
        ir_2 = urx.ImpulseResponse(
            42, np.nan, "s", [3.14, -42])
        ir_3 = urx.ImpulseResponse(
            123, 4.56, "ms", [1, 2, 3])

        elt = urx.Element()
        elt_2 = urx.Element(t_2, eg_2, ir_2)

        # Check CTOR with all parameters
        p_rca = urx.Probe("rca probe", urx.ProbeType.RCA, t, urx.VecElementGeometryPtr([
            eg, eg_2]), urx.VecImpulseResponsePtr([ir, ir_2]), urx.VecElement([elt, elt_2]))
        p_rca = urx.Probe("rca probe", urx.ProbeType.RCA, t, [
                          eg, eg_2], [ir, ir_2], [elt, elt_2])

        # Instantiate second object to test == assertion
        p_rca_bis = urx.Probe(p_rca)

        # element_geometries is a pointer vector, thus all modifications are shared
        self.assertEqual(p_rca.element_geometries[1], eg_2)
        self.assertEqual(p_rca.elements[1].element_geometry, eg_2)
        p_rca.element_geometries[1].perimeter[0] = v_3
        self.assertEqual(p_rca, p_rca_bis)
        self.assertEqual(p_rca.element_geometries[1], eg_2)
        self.assertEqual(p_rca.elements[1].element_geometry, eg_2)
        self.assertEqual(p_rca_bis.element_geometries[1], eg_2)
        self.assertEqual(p_rca_bis.elements[1].element_geometry, eg_2)

        # Reference is possible for shared pointers vector
        self.assertEqual(p_rca.element_geometries, [eg, eg_2])
        element_geometries_ref = p_rca.element_geometries
        element_geometries_ref[0] = eg_3
        self.assertEqual(p_rca.element_geometries, element_geometries_ref)
        self.assertNotEqual(p_rca, p_rca_bis)
        # Check assignment
        p_rca.element_geometries = [eg, eg_2]
        self.assertEqual(p_rca.element_geometries, element_geometries_ref)
        self.assertEqual(p_rca.element_geometries, [eg, eg_2])
        self.assertEqual(p_rca, p_rca_bis)

        # impulse_responses is a pointer vector, thus all modifications are shared
        p_rca.impulse_responses[1].sampling_frequency.value = 789.123
        self.assertEqual(p_rca, p_rca_bis)
        self.assertEqual(p_rca.impulse_responses[1], ir_2)
        self.assertEqual(p_rca_bis.impulse_responses[1], ir_2)

        # Reference is possible for shared pointers vector
        self.assertEqual(p_rca.impulse_responses, [ir, ir_2])
        impulse_responses_ref = p_rca.impulse_responses
        impulse_responses_ref[0] = ir_3
        self.assertEqual(p_rca.impulse_responses, impulse_responses_ref)
        self.assertNotEqual(p_rca, p_rca_bis)
        # Check assignment
        p_rca.impulse_responses = [ir, ir_2]
        self.assertEqual(p_rca.impulse_responses, impulse_responses_ref)
        self.assertEqual(p_rca.impulse_responses, [ir, ir_2])
        self.assertEqual(p_rca, p_rca_bis)

        # deleting the local variable does not impact the shared pointers vectors
        del eg_2
        del ir_2
        gc.collect()
        self.assertEqual(len(p_rca.element_geometries), 2)
        self.assertEqual(len(p_rca_bis.element_geometries), 2)
        self.assertEqual(len(p_rca.impulse_responses), 2)
        self.assertEqual(len(p_rca_bis.impulse_responses), 2)

        # deleting the pointers inside the vectors to check resize
        del p_rca.element_geometries[1]
        del p_rca_bis.element_geometries[1]
        del p_rca.impulse_responses[1]
        del p_rca_bis.impulse_responses[1]
        gc.collect()
        self.assertEqual(len(p_rca.element_geometries), 1)
        self.assertEqual(len(p_rca_bis.element_geometries), 1)
        self.assertEqual(len(p_rca.impulse_responses), 1)
        self.assertEqual(len(p_rca_bis.impulse_responses), 1)
        self.assertRaises(
            RuntimeError, lambda p_rca: p_rca.elements[1].element_geometry, p_rca)
        self.assertRaises(
            RuntimeError, lambda p_rca_bis: p_rca_bis.elements[1].element_geometry, p_rca_bis)
        self.assertRaises(
            RuntimeError, lambda p_rca: p_rca.elements[1].impulse_response, p_rca)
        self.assertRaises(
            RuntimeError, lambda p_rca_bis: p_rca_bis.elements[1].impulse_response, p_rca_bis)

        # Reference is not possible for enum
        self.assertEqual(p_rca.type, urx.ProbeType.RCA)
        type_2 = p_rca.type
        type_2 = urx.ProbeType.LINEAR
        self.assertNotEqual(p_rca.type, type_2)
        self.assertEqual(p_rca, p_rca_bis)
        # Check assignment
        p_rca.type = urx.ProbeType.RCA
        self.assertEqual(p_rca.type, urx.ProbeType.RCA)
        self.assertEqual(p_rca, p_rca_bis)

        # Reference is not possible for string
        self.assertEqual(p_rca.description, "rca probe")
        description_2 = p_rca.description
        description_2 = "Hello"
        self.assertNotEqual(p_rca.description, description_2)
        self.assertEqual(p_rca, p_rca_bis)
        # Check assignment
        p_rca.description = "rca probe"
        self.assertEqual(p_rca.description, "rca probe")
        self.assertEqual(p_rca, p_rca_bis)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
