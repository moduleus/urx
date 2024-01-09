import pyurx as urx
import numpy as np
import unittest
import gc


class TestBindingsWave(unittest.TestCase):

    def testProbe(self):
        testName = "Wave"
        print("\n--Test %s binding BEGIN--" % testName)

        # Check default CTOR
        w = urx.Wave()
        self.assertEqual(w.type, urx.WaveType.UNDEFINED)
        self.assertEqual(w.time_zero, urx.DoubleNan())
        self.assertEqual(w.time_zero_reference_point, urx.Vector3D())
        self.assertEqual(w.channel_mapping, urx.VecVecUInt32())
        self.assertEqual(w.channel_excitations, urx.VecExcitationPtr())
        self.assertEqual(w.channel_delays, urx.VecFloat64())
        self.assertEqual(w.parameters, urx.VecFloat64())

        # Check copy CTOR and referencing object
        w_2 = urx.Wave(w)
        self.assertEqual(w, w_2)
        w_2.type = urx.WaveType.CONVERGING_WAVE
        self.assertNotEqual(w, w_2)
        p_ref = w
        p_ref.type = urx.WaveType.CONVERGING_WAVE
        self.assertEqual(w, p_ref)

        ex = urx.Excitation("linear", urx.DoubleNan(42), np.nan, [3.14, -42])
        ex_2 = urx.Excitation(ex)

        ex_3 = urx.Excitation("linear", 123, 456, [111, 222])
        ex_4 = urx.Excitation(ex)

        # Check CTOR with all parameters
        w = urx.Wave(urx.WaveType.CONVERGING_WAVE,
                     0, urx.Vector3D(), [[1, 2, 3], [4, 5, 6, 7, 8, 9]], [ex, ex_2], [3.14, 42], [12.34, 56.78])
        self.assertEqual(w.type, urx.WaveType.CONVERGING_WAVE)
        self.assertEqual(w.time_zero, 0)
        self.assertEqual(w.time_zero_reference_point, urx.Vector3D())
        self.assertEqual(w.channel_mapping, [[1, 2, 3], [4, 5, 6, 7, 8, 9]])
        self.assertEqual(w.channel_excitations, [ex, ex_2])
        self.assertEqual(w.channel_delays,  [3.14, 42])
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

        # Reference is possible for channel_mapping (VecVecUInt32)
        self.assertEqual(w.channel_mapping,  [[1, 2, 3], [4, 5, 6, 7, 8, 9]])
        channel_mapping_ref = w.channel_mapping
        channel_mapping_ref[0] = [10, 11]
        self.assertEqual(w.channel_mapping, channel_mapping_ref)
        self.assertNotEqual(w, w_2)
        # Check assignment
        w.channel_mapping = [[1, 2, 3], [4, 5, 6, 7, 8, 9]]
        self.assertEqual(w.channel_mapping,  [[1, 2, 3], [4, 5, 6, 7, 8, 9]])
        self.assertEqual(w, w_2)

        # Vector of weak pointers cannot be referenced, thus a copy is made
        self.assertEqual(w.channel_excitations, [ex, ex_2])
        channel_excitations_2 = w.channel_excitations
        channel_excitations_2[0] = ex_3
        self.assertNotEqual(w.channel_excitations, channel_excitations_2)
        self.assertEqual(w, w_2)
        # Check assignment
        w.channel_excitations = [ex_3, ex_4, ex_2]
        self.assertEqual(w.channel_excitations, [ex_3, ex_4, ex_2])
        self.assertNotEqual(w, w_2)
        w_2 = urx.Wave(w)
        self.assertEqual(w, w_2)

        del ex_4
        gc.collect()

        self.assertEqual(len(w.channel_excitations), 3)
        self.assertEqual(len(w_2.channel_excitations), 3)

        self.assertIsNone(w.channel_excitations[1])
        self.assertIsNone(w_2.channel_excitations[1])

        # Reference is possible for channel_delays (VecFloat64)
        self.assertEqual(w.channel_delays, [3.14, 42])
        channel_delays_ref = w.channel_delays
        channel_delays_ref[0] = 123
        self.assertEqual(w.channel_delays, channel_delays_ref)
        self.assertNotEqual(w, w_2)
        # Check assignment
        w.channel_delays = [3.14, 42]
        self.assertEqual(w.channel_delays, [3.14, 42])
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
