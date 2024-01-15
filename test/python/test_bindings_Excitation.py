import pyurx as urx
import numpy as np
import unittest


class TestBindingsExcitation(unittest.TestCase):

    def testExcitation(self):
        testName = "Excitation binding"
        print("\n--Test %s BEGIN--" % testName)

        # Check default CTOR
        ex = urx.Excitation()
        self.assertEqual(ex.pulse_shape, "")
        self.assertTrue(np.isnan(ex.transmit_frequency.value))
        self.assertTrue(np.isnan(ex.sampling_frequency.value))
        self.assertEqual(len(ex.waveform), 0)

        # Check copy CTOR and referencing object
        ex_2 = urx.Excitation(ex)
        self.assertEqual(ex, ex_2)
        ex_2.pulse_shape = "Hello"
        self.assertNotEqual(ex, ex_2)
        ex_ref = ex
        ex_ref.pulse_shape = "Hello"
        self.assertEqual(ex, ex_ref)

        # Check CTOR with all parameters
        ex = urx.Excitation("linear", 42, np.nan, urx.VecFloat64([3.14, -42]))
        self.assertEqual(ex, urx.Excitation(
            "linear", 42.0, np.nan, [3.14, -42]))
        ex_2 = urx.Excitation(ex)
        self.assertEqual(ex, ex_2)

        # Check CTOR with all parameters varying double and DoubleNan
        ex = urx.Excitation("linear", urx.DoubleNan(42), np.nan, [3.14, -42])
        self.assertEqual(ex, ex_2)
        ex = urx.Excitation("linear", 42.0, urx.DoubleNan(np.nan), [3.14, -42])
        self.assertEqual(ex, ex_2)
        ex = urx.Excitation("linear", urx.DoubleNan(42), urx.DoubleNan(
            np.nan), urx.VecFloat64([3.14, -42]))
        self.assertEqual(ex, ex_2)

        # Reference is possible for waveform (VecFloat64)
        self.assertEqual(ex, ex_2)
        waveform_ref = ex.waveform
        self.assertEqual(waveform_ref, [3.14, -42])
        waveform_ref[0] = 123.456
        self.assertEqual(waveform_ref, ex.waveform)
        waveform_ref.append(987)
        self.assertEqual(waveform_ref, ex.waveform)
        ex.waveform.append(852)
        self.assertEqual(waveform_ref, ex.waveform)
        self.assertNotEqual(ex, ex_2)
        # Check assignment
        ex.waveform = [3.14, -42]
        self.assertEqual(waveform_ref, [3.14, -42])

        # Reference is possible for transmit_frequency (DoubleNan)
        self.assertEqual(ex, ex_2)
        ex.transmit_frequency = urx.DoubleNan(np.nan)
        transmit_frequency_ref = ex.transmit_frequency
        self.assertEqual(transmit_frequency_ref, ex.transmit_frequency)
        self.assertEqual(transmit_frequency_ref, urx.DoubleNan(np.nan))
        self.assertNotEqual(transmit_frequency_ref.value, np.nan)
        # Check assignment
        ex.transmit_frequency = urx.DoubleNan(10)
        self.assertEqual(transmit_frequency_ref, ex.transmit_frequency)
        transmit_frequency_ref += 12
        self.assertEqual(transmit_frequency_ref, ex.transmit_frequency)
        transmit_frequency_ref -= 12
        self.assertEqual(transmit_frequency_ref, ex.transmit_frequency)
        transmit_frequency_ref *= 12
        self.assertEqual(transmit_frequency_ref, ex.transmit_frequency)
        transmit_frequency_ref /= 12
        self.assertEqual(transmit_frequency_ref, ex.transmit_frequency)
        transmit_frequency_ref = 123
        self.assertNotEqual(transmit_frequency_ref, ex.transmit_frequency)
        self.assertNotEqual(ex, ex_2)
        ex.transmit_frequency = urx.DoubleNan(42)

        # Reference is possible for sampling_frequency (DoubleNan)
        self.assertEqual(ex, ex_2)
        ex.sampling_frequency = urx.DoubleNan(np.nan)
        sampling_frequency_ref = ex.sampling_frequency
        self.assertEqual(sampling_frequency_ref, ex.sampling_frequency)
        self.assertEqual(sampling_frequency_ref, urx.DoubleNan(np.nan))
        self.assertNotEqual(sampling_frequency_ref.value, np.nan)
        # Check assignment
        ex.sampling_frequency.value = 10
        self.assertEqual(sampling_frequency_ref, ex.sampling_frequency)
        sampling_frequency_ref += 12
        self.assertEqual(sampling_frequency_ref, ex.sampling_frequency)
        sampling_frequency_ref -= 12
        self.assertEqual(sampling_frequency_ref, ex.sampling_frequency)
        sampling_frequency_ref *= 12
        self.assertEqual(sampling_frequency_ref, ex.sampling_frequency)
        sampling_frequency_ref /= 12
        self.assertEqual(sampling_frequency_ref, ex.sampling_frequency)
        sampling_frequency_ref = urx.DoubleNan(123)
        self.assertNotEqual(sampling_frequency_ref, ex.sampling_frequency)
        self.assertNotEqual(ex, ex_2)
        ex.sampling_frequency = urx.DoubleNan()

        # Reference is not possible for pulse_shape (string)
        self.assertEqual(ex, ex_2)
        pulse_shape = ex.pulse_shape
        self.assertEqual(pulse_shape, ex.pulse_shape)
        ex.pulse_shape = "Hello"
        self.assertNotEqual(pulse_shape, ex.pulse_shape)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
