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
        self.assertRaises(
            RuntimeError, lambda ts: ts.wave, ts)
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

        # Check CTOR with all parameters
        ts = urx.TransmitSetup(urx.Probe(), urx.Wave(), t_2, 42)
        self.assertRaises(
            RuntimeError, lambda ts: ts.probe, ts)
        self.assertRaises(
            RuntimeError, lambda ts: ts.wave, ts)

        probe = urx.Probe()
        wave = urx.Wave()
        probe_2 = urx.Probe(probe)
        probe_2.description = 'rca'
        self.assertNotEqual(probe, probe_2)

        wave_2 = urx.Wave(wave)
        wave_2.type = urx.WaveType.PLANE_WAVE
        self.assertNotEqual(wave, wave_2)

        ts = urx.TransmitSetup(probe_2, wave_2, t_2, 42)
        self.assertEqual(ts.probe, probe_2)
        self.assertEqual(ts.wave, wave_2)
        self.assertEqual(ts.probe_transform, t_2)
        self.assertEqual(ts.time_offset, 42)

        ts = urx.TransmitSetup(probe, wave, t_2, 42)

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

        # wave is a pointer and thus shared between objects
        self.assertEqual(ts.wave, wave)
        ts.wave = wave_2
        self.assertEqual(ts.wave, wave_2)
        wave_2.type = urx.WaveType.DIVERGING_WAVE
        self.assertEqual(ts.wave, wave_2)
        del wave_2
        gc.collect()
        self.assertRaises(
            RuntimeError, lambda ts: ts.wave, ts)

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
