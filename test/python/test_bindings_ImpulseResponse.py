import pyurx as urx
import numpy as np
import unittest


class TestBindingsImpulseResponse(unittest.TestCase):

    def testImpulseResponse(self):
        testName = "ImpulseResponse binding"
        print("\n--Test %s BEGIN--" % testName)

        ir = urx.ImpulseResponse()
        ir_2 = urx.ImpulseResponse()

        self.assertTrue(np.isnan(ir.sampling_frequency.value))
        self.assertEqual(ir.time_offset.value, 0)
        self.assertEqual(ir.units, "")
        self.assertEqual(len(ir.data), 0)
        self.assertEqual(ir, ir_2)

        ir = urx.ImpulseResponse(42, np.nan, "sec", [3.14, -42])
        self.assertEqual(ir, urx.ImpulseResponse(
            42, np.nan, "sec", [3.14, -42]))
        ir_2 = urx.ImpulseResponse(ir)
        self.assertEqual(ir, ir_2)

        ir = urx.ImpulseResponse(urx.DoubleNan(42), np.nan, "sec", [3.14, -42])
        self.assertEqual(ir, ir_2)
        ir = urx.ImpulseResponse(42, urx.DoubleNan(np.nan), "sec", [3.14, -42])
        self.assertEqual(ir, ir_2)
        ir = urx.ImpulseResponse(urx.DoubleNan(42), urx.DoubleNan(
            np.nan), "sec", np.array([3.14, -42]))
        self.assertEqual(ir, ir_2)

        data = ir.data
        self.assertEqual(data, [3.14, -42])
        data[0] = 123.456
        self.assertEqual(data, ir.data)

        time_offset = ir.time_offset
        self.assertEqual(time_offset, np.nan)
        self.assertEqual(time_offset.value, np.nan)

        ir.time_offset = 10
        self.assertEqual(time_offset, ir.time_offset)
        time_offset += 12
        self.assertEqual(time_offset, ir.time_offset)
        time_offset -= 12
        self.assertEqual(time_offset, ir.time_offset)
        time_offset *= 12
        self.assertEqual(time_offset, ir.time_offset)
        time_offset /= 12
        self.assertEqual(time_offset, ir.time_offset)
        time_offset = 123
        self.assertNotEqual(time_offset, ir.time_offset)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
