import pyurx as urx
import numpy as np
from time import perf_counter

import unittest
from dataclasses import dataclass

@dataclass
class DoubleNanWrapper:    
    d: urx.DoubleNan
    def __init__(self):
        self.d = urx.DoubleNan()


class TestBindingsDoubleNan(unittest.TestCase):

    def testDoubleNan(self):
        testName = "DoubleNan binding"
        print("\n--Test %s BEGIN--" % testName)

        d = urx.DoubleNan()
        dw = DoubleNanWrapper()

        self.assertTrue(np.isnan(d.value))
        self.assertEqual(d, dw.d)
        self.assertNotEqual(d.value, dw.d.value)
        d.value = 42
        self.assertNotEqual(d, dw.d)
        self.assertNotEqual(d.value, dw.d.value)
        dw.d = urx.DoubleNan(42)
        self.assertEqual(d, dw.d)
        self.assertEqual(d.value, dw.d.value)

        print("--Test %s END--" % testName)

if __name__ == '__main__':
    unittest.main()
