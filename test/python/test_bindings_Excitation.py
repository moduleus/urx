import pyurx as urx
import numpy as np
import unittest


class TestBindingsExcitation(unittest.TestCase):

    def testExcitation(self):
        testName = "Excitation binding"
        print("\n--Test %s BEGIN--" % testName)

        ex = urx.Excitation()
        ex_2 = urx.Excitation()

        self.assertEqual(ex.pulse_shape, "")
        self.assertTrue(np.isnan(ex.transmit_frequency.value))
        self.assertTrue(np.isnan(ex.sampling_frequency.value))
        self.assertEqual(len(ex.waveform), 0)
        self.assertEqual(ex, ex_2)

        ex = urx.Excitation("linear", 42, np.nan, urx.VecFloat64([3.14, -42]))
        self.assertEqual(ex, urx.Excitation("linear", 42, np.nan, [3.14, -42]))
        ex_2 = urx.Excitation(ex)
        self.assertEqual(ex, ex_2)

        ex = urx.Excitation("linear", urx.DoubleNan(42), np.nan, [3.14, -42])
        self.assertEqual(ex, ex_2)
        ex = urx.Excitation("linear", 42, urx.DoubleNan(np.nan), [3.14, -42])
        self.assertEqual(ex, ex_2)
        ex = urx.Excitation("linear", urx.DoubleNan(42), urx.DoubleNan(
            np.nan), urx.VecFloat64([3.14, -42]))
        self.assertEqual(ex, ex_2)

        waveform = ex.waveform
        self.assertEqual(waveform, [3.14, -42])
        waveform[0] = 123.456
        self.assertEqual(waveform, ex.waveform)
        waveform.append(987)
        self.assertEqual(waveform, ex.waveform)
        ex.waveform.append(852)
        self.assertEqual(waveform, ex.waveform)

        ex.transmit_frequency = urx.DoubleNan(np.nan)
        transmit_frequency = ex.transmit_frequency
        self.assertEqual(transmit_frequency, ex.transmit_frequency)
        self.assertEqual(transmit_frequency, urx.DoubleNan(np.nan))
        self.assertNotEqual(transmit_frequency.value, np.nan)
        ex.transmit_frequency = urx.DoubleNan(10)
        self.assertEqual(transmit_frequency, ex.transmit_frequency)
        transmit_frequency += 12
        self.assertEqual(transmit_frequency, ex.transmit_frequency)
        transmit_frequency -= 12
        self.assertEqual(transmit_frequency, ex.transmit_frequency)
        transmit_frequency *= 12
        self.assertEqual(transmit_frequency, ex.transmit_frequency)
        transmit_frequency /= 12
        self.assertEqual(transmit_frequency, ex.transmit_frequency)
        transmit_frequency = 123
        self.assertNotEqual(transmit_frequency, ex.transmit_frequency)

        ex.sampling_frequency = urx.DoubleNan(np.nan)
        sampling_frequency = ex.sampling_frequency
        self.assertEqual(sampling_frequency, ex.sampling_frequency)
        self.assertEqual(sampling_frequency, urx.DoubleNan(np.nan))
        self.assertNotEqual(sampling_frequency.value, np.nan)
        ex.sampling_frequency.value = 10
        self.assertEqual(sampling_frequency, ex.sampling_frequency)
        sampling_frequency += 12
        self.assertEqual(sampling_frequency, ex.sampling_frequency)
        sampling_frequency -= 12
        self.assertEqual(sampling_frequency, ex.sampling_frequency)
        sampling_frequency *= 12
        self.assertEqual(sampling_frequency, ex.sampling_frequency)
        sampling_frequency /= 12
        self.assertEqual(sampling_frequency, ex.sampling_frequency)
        sampling_frequency = urx.DoubleNan(123)
        self.assertNotEqual(sampling_frequency, ex.sampling_frequency)

        # No ref possible for string
        pulse_shape = ex.pulse_shape
        self.assertEqual(pulse_shape, ex.pulse_shape)
        ex.pulse_shape = "Hello"
        self.assertNotEqual(pulse_shape, ex.pulse_shape)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
