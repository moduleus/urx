import pyurx as urx
import numpy as np
from time import perf_counter

import unittest
from parameterized import parameterized


class TestBindingsDataset(unittest.TestCase):

    def testDataset(self):
        testName = "Dataset binding"
        print("\n--Test %s BEGIN--" % testName)

        dataset = urx.Dataset()
        acq = urx.Acquisition()
        version = urx.Version()

        self.assertEqual(dataset.acquisition, acq)
        dataset.acquisition.timestamp = 42
        acq.timestamp = 42
        self.assertEqual(dataset.acquisition, acq)

        self.assertEqual(dataset.version, version)
        dataset.version.major = 42
        self.assertNotEqual(dataset.version, version)
        version.major = 42
        self.assertEqual(dataset.version, version)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
