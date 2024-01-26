import pyurx as urx
import numpy as np
import unittest
import gc


class TestBindingsTransmitSetup(unittest.TestCase):

    def testTransmitSetup(self):
        testName = "TransmitSetup"
        print("\n--Test %s binding BEGIN--" % testName)

        # Check default CTOR
        ts = urx.TransmitSetup()
        self.assertRaises(
            RuntimeError, lambda ts: ts.probe, ts)
        self.assertEqual(ts.active_elements, urx.VecVecUInt32())
        self.assertEqual(ts.excitations, urx.VecExcitationPtr())
        self.assertEqual(ts.delays, urx.VecFloat64())
        self.assertEqual(ts.probe_transform, urx.Transform())
        self.assertEqual(ts.time_offset, urx.DoubleNan(0))

        v = urx.Vector3D()
        v_2 = urx.Vector3D(1, 2, 3)

        t_2 = urx.Transform(v_2, v_2)

        # Check copy CTOR and referencing object
        ts_2 = urx.TransmitSetup(ts)
        self.assertEqual(ts, ts_2)
        ts_2.probe_transform = t_2
        self.assertNotEqual(ts, ts_2)
        ts_ref = ts
        ts_ref.probe_transform = t_2
        self.assertEqual(ts, ts_ref)

        ex = urx.Excitation("linear", urx.DoubleNan(42), np.nan, [3.14, -42])
        ex_2 = urx.Excitation(ex)

        ex_3 = urx.Excitation("linear", 123, 456, [111, 222])
        ex_4 = urx.Excitation(ex)

        # Check CTOR with all parameters
        ts = urx.TransmitSetup(urx.Probe(), urx.Wave(), [[1, 2, 3], [
                               4, 5, 6, 7, 8, 9]], [ex, ex_2], [3.14, 42], t_2, 42)
        ts_2 = urx.TransmitSetup(ts)
        self.assertRaises(
            RuntimeError, lambda ts: ts.probe, ts)
        self.assertEqual(ts.active_elements, [[1, 2, 3], [4, 5, 6, 7, 8, 9]])
        self.assertEqual(ts.excitations, [ex, ex_2])
        self.assertEqual(ts.delays,  [3.14, 42])

        probe = urx.Probe()
        wave = urx.Wave()
        probe_2 = urx.Probe(probe)
        probe_2.description = 'rca'
        self.assertNotEqual(probe, probe_2)

        wave_2 = urx.Wave(wave)
        wave_2.type = urx.WaveType.PLANE_WAVE
        self.assertNotEqual(wave, wave_2)

        ts = urx.TransmitSetup(probe_2, wave_2, [[1, 2, 3], [
                               4, 5, 6, 7, 8, 9]], [ex, ex_2], [3.14, 42], t_2, 42)
        self.assertEqual(ts.probe, probe_2)
        self.assertEqual(ts.wave, wave_2)
        self.assertEqual(ts.probe_transform, t_2)
        self.assertEqual(ts.time_offset, 42)

        ts = urx.TransmitSetup(probe, wave, [[1, 2, 3], [
                               4, 5, 6, 7, 8, 9]], [ex, ex_2], [3.14, 42], t_2, 42)

        # probe is a pointer and thus shared between objects
        self.assertEqual(ts.probe, probe)
        ts.probe = probe_2
        self.assertEqual(ts.probe, probe_2)
        probe_2.description = "linear"
        self.assertEqual(ts.probe, probe_2)
        del probe_2
        gc.collect()
        self.assertRaises(
            RuntimeError, lambda ts: ts.probe, ts)

        # wave is not a pointer
        self.assertEqual(ts.wave, wave)
        ts.wave = wave_2
        self.assertEqual(ts.wave, wave_2)
        wave_2.type = urx.WaveType.DIVERGING_WAVE
        self.assertNotEqual(ts.wave, wave_2)
        del wave_2
        gc.collect()
        ts_2.wave.type = urx.WaveType.PLANE_WAVE

        # Reference is possible for active_elements (VecVecUInt32)
        self.assertEqual(ts.active_elements,  [[1, 2, 3], [4, 5, 6, 7, 8, 9]])
        channel_mapping_ref = ts.active_elements
        channel_mapping_ref[0] = [10, 11]
        self.assertEqual(ts.active_elements, channel_mapping_ref)
        self.assertNotEqual(ts, ts_2)
        # Check assignment
        ts.active_elements = [[1, 2, 3], [4, 5, 6, 7, 8, 9]]
        self.assertEqual(ts.active_elements,  [[1, 2, 3], [4, 5, 6, 7, 8, 9]])
        self.assertEqual(ts, ts_2)

        # Vector of weak pointers cannot be referenced, thus a copy is made
        self.assertEqual(ts.excitations, [ex, ex_2])
        channel_excitations_2 = ts.excitations
        channel_excitations_2[0] = ex_3
        self.assertNotEqual(ts.excitations, channel_excitations_2)
        self.assertEqual(ts, ts_2)
        # Check assignment
        ts.excitations = [ex_3, ex_4, ex_2]
        self.assertEqual(ts.excitations, [ex_3, ex_4, ex_2])
        self.assertNotEqual(ts, ts_2)
        ts_2 = urx.TransmitSetup(ts)
        self.assertEqual(ts, ts_2)

        del ex_4
        gc.collect()

        self.assertEqual(len(ts.excitations), 3)
        self.assertEqual(len(ts_2.excitations), 3)

        self.assertIsNone(ts.excitations[1])
        self.assertIsNone(ts_2.excitations[1])

        # Reference is possible for delays (VecFloat64)
        self.assertEqual(ts.delays, [3.14, 42])
        channel_delays_ref = ts.delays
        channel_delays_ref[0] = 123
        self.assertEqual(ts.delays, channel_delays_ref)
        self.assertNotEqual(ts, ts_2)
        # Check assignment
        ts.delays = [3.14, 42]
        self.assertEqual(ts.delays, [3.14, 42])
        self.assertEqual(ts, ts_2)

        # Reference is possible for probe_transform
        ts_2 = urx.TransmitSetup(ts)
        self.assertEqual(ts.probe_transform, t_2)
        t_2.rotation = v
        self.assertEqual(ts, ts_2)
        self.assertEqual(ts.probe_transform, urx.Transform(v_2, v_2))
        self.assertNotEqual(ts.probe_transform, t_2)
        t_ref = ts.probe_transform
        t_ref.rotation = v
        self.assertEqual(ts.probe_transform, t_ref)
        self.assertNotEqual(ts, ts_2)
        # Check assignment
        ts.probe_transform = urx.Transform(v_2, v_2)
        self.assertEqual(ts.probe_transform, t_ref)
        self.assertEqual(ts.probe_transform, urx.Transform(v_2, v_2))
        self.assertEqual(ts, ts_2)

        # Reference is possible for time_offset (DoubleNan)
        self.assertEqual(ts.time_offset, 42)
        time_offset_ref = ts.time_offset
        time_offset_ref.value = 123
        self.assertEqual(ts.time_offset,
                         time_offset_ref)
        self.assertNotEqual(ts, ts_2)
        # Check assignment
        ts.time_offset = urx.DoubleNan(42)
        self.assertEqual(ts.time_offset,
                         time_offset_ref)
        self.assertEqual(ts.time_offset, 42)
        self.assertEqual(ts, ts_2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
