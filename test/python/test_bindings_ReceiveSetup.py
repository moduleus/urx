import pyurx as urx
import unittest
import gc


class TestBindingsReceiveSetup(unittest.TestCase):

    def testReceiveSetup(self):
        testName = "ReceiveSetup"
        print("\n--Test %s binding BEGIN--" % testName)

        # Check default CTOR
        rs = urx.ReceiveSetup()
        self.assertRaises(
            RuntimeError, lambda rs: rs.probe, rs)
        self.assertEqual(rs.probe_transform, urx.Transform())
        self.assertEqual(rs.sampling_frequency, urx.DoubleNan())
        self.assertEqual(rs.number_samples, 0)
        self.assertEqual(rs.active_elements, urx.VecVecUInt32())
        self.assertEqual(rs.tgc_profile, urx.VecFloat64())
        self.assertEqual(rs.tgc_sampling_frequency, urx.DoubleNan())
        self.assertEqual(rs.modulation_frequency, urx.DoubleNan())
        self.assertEqual(rs.time_offset, urx.DoubleNan(0))

        rs_2 = urx.ReceiveSetup(rs)

        v = urx.Vector3D()
        v_2 = urx.Vector3D(1, 2, 3)

        t_2 = urx.Transform(v_2, v_2)

        # Check copy CTOR and referencing object
        self.assertEqual(rs, rs_2)
        rs_2.probe_transform = t_2
        self.assertNotEqual(rs, rs_2)
        rs_ref = rs
        rs_ref.probe_transform = t_2
        self.assertEqual(rs, rs_ref)

        # Check CTOR with all parameters
        rs = urx.ReceiveSetup(urx.Probe(), t_2, 1, 2, [[3]], [4], 5, 6, 7)
        self.assertRaises(
            RuntimeError, lambda rs: rs.probe, rs)

        probe = urx.Probe()
        probe_2 = urx.Probe(probe)
        probe_2.description = 'rca'
        self.assertNotEqual(probe, probe_2)

        rs = urx.ReceiveSetup(probe_2, t_2, 1, 2, [[3]], [4], 5, 6, 7)
        rs_2 = urx.ReceiveSetup(rs)
        self.assertEqual(rs, rs_2)

        # Reference is not possible for string
        self.assertEqual(rs.probe, probe_2)
        # Check assignment
        probe_2.description = "linear"
        self.assertEqual(rs, rs_2)
        self.assertEqual(rs.probe, probe_2)
        del probe_2
        gc.collect()
        self.assertRaises(
            RuntimeError, lambda rs: rs.probe, rs)

        # Reference is possible for probe_transform
        self.assertEqual(rs.probe_transform, t_2)
        t_2.rotation = v
        self.assertEqual(rs, rs_2)
        self.assertEqual(rs.probe_transform, urx.Transform(v_2, v_2))
        self.assertNotEqual(rs.probe_transform, t_2)
        t_ref = rs.probe_transform
        t_ref.rotation = v
        self.assertEqual(rs.probe_transform, t_ref)
        self.assertNotEqual(rs, rs_2)
        # Check assignment
        rs.probe_transform = urx.Transform(v_2, v_2)
        self.assertEqual(rs.probe_transform, t_ref)
        self.assertEqual(rs.probe_transform, urx.Transform(v_2, v_2))
        self.assertEqual(rs, rs_2)

        # Reference is possible for sampling_frequency (DoubleNan)
        self.assertEqual(rs.sampling_frequency, 1)
        sampling_frequency_ref = rs.sampling_frequency
        sampling_frequency_ref.value = 123
        self.assertEqual(rs.sampling_frequency, sampling_frequency_ref)
        self.assertNotEqual(rs, rs_2)
        # Check assignment
        rs.sampling_frequency = urx.DoubleNan(1)
        self.assertEqual(rs.sampling_frequency, sampling_frequency_ref)
        self.assertEqual(rs.sampling_frequency, 1)
        self.assertEqual(rs, rs_2)

        # Reference is not possible for number_samples since it is a primitive type (uint32)
        self.assertEqual(rs.number_samples, 2)
        number_samples_2 = rs.number_samples
        number_samples_2 = 123
        self.assertNotEqual(rs.number_samples, number_samples_2)
        self.assertEqual(rs.number_samples, 2)
        rs.number_samples = 123
        self.assertNotEqual(rs, rs_2)
        # Check assignment
        rs.number_samples = 2
        self.assertEqual(rs, rs_2)

        # Reference is possible for active_elements (VecVecUInt32)
        self.assertEqual(rs.active_elements, [[3]])
        channel_mapping_ref = rs.active_elements
        channel_mapping_ref[0] = [10, 11]
        self.assertEqual(rs.active_elements, channel_mapping_ref)
        self.assertNotEqual(rs, rs_2)
        # Check assignment
        rs.active_elements = [[3]]
        self.assertEqual(rs.active_elements, [[3]])
        self.assertEqual(rs, rs_2)

        # Reference is possible for tgc_profile (VecFloat64)
        self.assertEqual(rs.tgc_profile, [4])
        tgc_profile_ref = rs.tgc_profile
        tgc_profile_ref[0] = 123
        self.assertEqual(rs.tgc_profile, tgc_profile_ref)
        self.assertNotEqual(rs, rs_2)
        # Check assignment
        rs.tgc_profile = [4]
        self.assertEqual(rs.tgc_profile, [4])
        self.assertEqual(rs, rs_2)

        # Reference is possible for tgc_sampling_frequency (DoubleNan)
        self.assertEqual(rs.tgc_sampling_frequency, 5)
        tgc_sampling_frequency_ref = rs.tgc_sampling_frequency
        tgc_sampling_frequency_ref.value = 123
        self.assertEqual(rs.tgc_sampling_frequency,
                         tgc_sampling_frequency_ref)
        self.assertNotEqual(rs, rs_2)
        # Check assignment
        rs.tgc_sampling_frequency = urx.DoubleNan(5)
        self.assertEqual(rs.tgc_sampling_frequency,
                         tgc_sampling_frequency_ref)
        self.assertEqual(rs.tgc_sampling_frequency, 5)
        self.assertEqual(rs, rs_2)

        # Reference is possible for tgc_sampling_frequency (DoubleNan)
        self.assertEqual(rs.modulation_frequency, 6)
        modulation_frequency_ref = rs.modulation_frequency
        modulation_frequency_ref.value = 123
        self.assertEqual(rs.modulation_frequency,
                         modulation_frequency_ref)
        self.assertNotEqual(rs, rs_2)
        # Check assignment
        rs.modulation_frequency = urx.DoubleNan(6)
        self.assertEqual(rs.modulation_frequency,
                         modulation_frequency_ref)
        self.assertEqual(rs.modulation_frequency, 6)
        self.assertEqual(rs, rs_2)

        # Reference is possible for time_offset (DoubleNan)
        self.assertEqual(rs.time_offset, 7)
        time_offset_ref = rs.time_offset
        time_offset_ref.value = 123
        self.assertEqual(rs.time_offset,
                         time_offset_ref)
        self.assertNotEqual(rs, rs_2)
        # Check assignment
        rs.time_offset = urx.DoubleNan(7)
        self.assertEqual(rs.time_offset,
                         time_offset_ref)
        self.assertEqual(rs.time_offset, 7)
        self.assertEqual(rs, rs_2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
