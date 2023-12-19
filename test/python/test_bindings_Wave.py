import pyurx as urx
import numpy as np
import unittest
import gc


class TestBindingsWave(unittest.TestCase):

    def testProbe(self):
        testName = "Wave"
        print("\n--Test %s binding BEGIN--" % testName)

        w = urx.Wave()
        self.assertEqual(w.type, urx.WaveType.UNDEFINED)
        self.assertEqual(w.time_zero, urx.DoubleNan())
        self.assertEqual(w.time_zero_reference_point, urx.Vector3D())
        self.assertEqual(w.channel_mapping, urx.VecVecUInt32())
        self.assertEqual(w.channel_excitations, urx.VecExcitationPtr())
        self.assertEqual(w.channel_delays, urx.VecFloat64())
        self.assertEqual(w.parameters, urx.VecFloat64())

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

        w = urx.Wave(urx.WaveType.CONVERGING_WAVE,
                     0, urx.Vector3D(), [[1, 2, 3], [4, 5, 6, 7, 8, 9]], [ex, ex_2], [3.14, 42], [12.34, 56.78])
        w_2 = urx.Wave(w)
        self.assertEqual(w, w_2)

        w.time_zero.value = 132
        self.assertNotEqual(w, w_2)
        w_2 = urx.Wave(w)

        w.time_zero_reference_point = urx.Vector3D(1, 2, 3)
        self.assertNotEqual(w, w_2)
        w_2 = urx.Wave(w)

        w.channel_mapping = [[10, 11], [20, 21, 23]]
        self.assertNotEqual(w, w_2)
        w_2 = urx.Wave(w)

        w.channel_excitations = [ex_3, ex_4]
        self.assertNotEqual(w, w_2)
        w_2 = urx.Wave(w)

        w.channel_excitations[0] = ex_2
        self.assertEqual(w, w_2)
        w_2 = urx.Wave(w)

        w.channel_delays = [7.89, 9.87]
        self.assertNotEqual(w, w_2)
        w_2 = urx.Wave(w)

        w.parameters = [7.89, 9.87]
        self.assertNotEqual(w, w_2)
        w_2 = urx.Wave(w)

        del ex_4
        gc.collect()

        self.assertEqual(len(w.channel_excitations), 2)
        self.assertEqual(len(w_2.channel_excitations), 2)

        self.assertIsNone(w.channel_excitations[1])
        self.assertIsNone(w_2.channel_excitations[1])

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
