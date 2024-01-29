import pyurx as urx
import unittest


class TestBindingsWave(unittest.TestCase):

    def testWave(self):
        testName = "Wave"
        print("\n--Test %s binding BEGIN--" % testName)

        # Check default CTOR
        w = urx.Wave()
        self.assertEqual(w.type, urx.WaveType.UNDEFINED)
        self.assertEqual(w.time_zero, urx.DoubleNan())
        self.assertEqual(w.time_zero_reference_point, urx.Vector3D())
        self.assertEqual(w.parameters, urx.VecFloat64())

        # Check copy CTOR and referencing object
        w_2 = urx.Wave(w)
        self.assertEqual(w, w_2)
        w_2.type = urx.WaveType.CONVERGING_WAVE
        self.assertNotEqual(w, w_2)
        p_ref = w
        p_ref.type = urx.WaveType.CONVERGING_WAVE
        self.assertEqual(w, p_ref)

        # Check CTOR with all parameters
        w = urx.Wave(urx.WaveType.CONVERGING_WAVE,
                     0, urx.Vector3D(), [12.34, 56.78])
        self.assertEqual(w.type, urx.WaveType.CONVERGING_WAVE)
        self.assertEqual(w.time_zero, 0)
        self.assertEqual(w.time_zero_reference_point, urx.Vector3D())
        self.assertEqual(w.parameters, [12.34, 56.78])
        w_2 = urx.Wave(w)
        self.assertEqual(w, w_2)

        # Reference is not possible for enum
        self.assertEqual(w.type, urx.WaveType.CONVERGING_WAVE)
        type_2 = w.type
        type_2 = urx.WaveType.DIVERGING_WAVE
        self.assertNotEqual(w.type, type_2)
        self.assertEqual(w, w_2)
        # Check assignment
        w.type = urx.WaveType.CONVERGING_WAVE
        self.assertEqual(w.type, urx.WaveType.CONVERGING_WAVE)
        self.assertEqual(w, w_2)

        # Reference is possible for time_zero (DoubleNan)
        self.assertEqual(w.time_zero, 0)
        time_zero_ref = w.time_zero
        time_zero_ref.value = 123
        self.assertEqual(w.time_zero, time_zero_ref)
        self.assertNotEqual(w, w_2)
        # Check assignment
        w.time_zero = urx.DoubleNan(132)
        self.assertEqual(w.time_zero, time_zero_ref)
        self.assertEqual(w.time_zero, 132)
        w.time_zero = urx.DoubleNan(0)
        self.assertEqual(w, w_2)

        # Reference is possible for time_zero_reference_point (Vector3D)
        self.assertEqual(w.time_zero_reference_point, urx.Vector3D())
        time_zero_reference_point_ref = w.time_zero_reference_point
        time_zero_reference_point_ref.x = 123
        self.assertEqual(w.time_zero_reference_point,
                         time_zero_reference_point_ref)
        self.assertNotEqual(w, w_2)
        # Check assignment
        w.time_zero_reference_point = urx.Vector3D()
        self.assertEqual(w.time_zero_reference_point,
                         time_zero_reference_point_ref)
        self.assertEqual(w.time_zero_reference_point, urx.Vector3D())
        self.assertEqual(w, w_2)

        # Reference is possible for parameters (VecFloat64)
        self.assertEqual(w.parameters, [12.34, 56.78])
        parameters_ref = w.parameters
        parameters_ref[0] = 123
        self.assertEqual(w.parameters, parameters_ref)
        self.assertNotEqual(w, w_2)
        # Check assignment
        w.parameters = [12.34, 56.78]
        self.assertEqual(w.parameters, [12.34, 56.78])
        self.assertEqual(w, w_2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
