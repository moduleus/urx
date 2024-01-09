import pyurx as urx
import numpy as np

import unittest
from dataclasses import dataclass


@dataclass
class DoubleNanWrapper:
    d: urx.DoubleNan

    def __init__(self):
        self.d = urx.DoubleNan()


class TestBindingsDoubleNan(unittest.TestCase):

    def testDoubleNan(self):
        testName = "DoubleNan"
        print("\n--Test %s binding BEGIN--" % testName)

        # Check CTORs
        d = urx.DoubleNan()
        d_2 = urx.DoubleNan(d)
        d_3 = urx.DoubleNan(42)
        dw = DoubleNanWrapper()

        # Check Nan comparison
        self.assertTrue(np.isnan(d.value))
        self.assertEqual(d, d_2)
        self.assertEqual(d, urx.DoubleNan(np.nan))
        self.assertNotEqual(d, np.nan)
        self.assertNotEqual(d.value, np.nan)
        self.assertEqual(d, dw.d)
        self.assertNotEqual(d.value, dw.d.value)

        # Check non Nan comparison
        d.value = 42
        self.assertNotEqual(d, dw.d)
        self.assertNotEqual(d.value, dw.d.value)
        self.assertEqual(d, d_3)
        dw.d = urx.DoubleNan(42)
        self.assertEqual(d, dw.d)
        self.assertEqual(d.value, dw.d.value)

        # Check operators
        self.assertEqual(+d, 42)
        self.assertEqual(-d, -42)
        d += 1
        self.assertEqual(d.value, 43)
        d -= 1
        self.assertEqual(d.value, 42)
        d *= 2
        self.assertEqual(d.value, 84)
        d /= 2
        self.assertEqual(d.value, 42)

        self.assertEqual(2.13+d, urx.DoubleNan(44.13))
        self.assertEqual(d+2.13, urx.DoubleNan(44.13))
        self.assertEqual(2+d, urx.DoubleNan(44))
        self.assertEqual(d+2, urx.DoubleNan(44))

        self.assertEqual(2.13-d, urx.DoubleNan(2.13-42))
        self.assertEqual(d-2.13, urx.DoubleNan(42-2.13))
        self.assertEqual(2-d, urx.DoubleNan(2-42))
        self.assertEqual(d-2, urx.DoubleNan(42-2))

        self.assertEqual(2.13*d, urx.DoubleNan(2.13*42))
        self.assertEqual(d*2.13, urx.DoubleNan(42*2.13))
        self.assertEqual(2*d, urx.DoubleNan(2*42))
        self.assertEqual(d*2, urx.DoubleNan(42*2))

        self.assertEqual(2.13/d, urx.DoubleNan(2.13/42))
        self.assertEqual(d/2.13, urx.DoubleNan(42/2.13))
        self.assertEqual(2/d, urx.DoubleNan(2/42))
        self.assertEqual(d/2, urx.DoubleNan(42/2))

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
