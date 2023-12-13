import pyurx as urx
import numpy as np
import unittest


class TestBindingsImpulseResponse(unittest.TestCase):

    def testImpulseResponse(self):
        testName = "ImpulseResponse binding"
        print("\n--Test %s BEGIN--" % testName)

        ir = urx.ImpulseResponse()
        ir2 = urx.ImpulseResponse()

        self.assertTrue(np.isnan(ir.sampling_frequency.value))
        self.assertEqual(ir.time_offset.value, 0)
        self.assertEqual(ir.units, "")
        self.assertEqual(len(ir.data), 0)
        self.assertEqual(ir, ir2)

        ir = urx.ImpulseResponse(42, np.nan, "sec", [3.14, -42])
        ir2 = urx.ImpulseResponse(42, np.nan, "sec", [3.14, -42])
        self.assertEqual(ir, ir2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
