import pyurx as urx
from time import perf_counter

import unittest


class TestBindingsDataset(unittest.TestCase):

    def testDataset(self):
        testName = "Dataset binding"
        print("\n--Test %s BEGIN--" % testName)

        # Check default CTOR
        dataset = urx.Dataset()
        self.assertEqual(dataset.acquisition, urx.Acquisition())
        self.assertEqual(dataset.version, urx.Version())

        acq = urx.Acquisition()
        acq.description = "Hello"

        v = urx.Version()
        v.major = 42

        # Check copy CTOR and referencing object
        dataset_2 = urx.Dataset(dataset)
        self.assertEqual(dataset, dataset_2)
        dataset_2.acquisition = acq
        self.assertNotEqual(dataset, dataset_2)
        dataset_ref = dataset
        dataset_ref.acquisition = acq
        self.assertEqual(dataset, dataset_ref)

        # Check CTOR with all parameters
        dataset = urx.Dataset(acq, v)
        self.assertEqual(dataset.acquisition, acq)
        self.assertEqual(dataset.version, v)
        dataset_2 = urx.Dataset(dataset)

        # Reference is possible for acquisition (Acquisition)
        self.assertEqual(dataset.acquisition, acq)
        acq_ref = dataset.acquisition
        acq_ref.description = "World"
        self.assertEqual(dataset.acquisition, acq_ref)
        self.assertNotEqual(dataset, dataset_2)
        # Check assignment
        dataset.acquisition = acq
        self.assertEqual(dataset.acquisition, acq_ref)
        self.assertEqual(dataset.acquisition, acq)
        self.assertEqual(dataset, dataset_2)

        # Reference is possible for version (Version)
        self.assertEqual(dataset.version, v)
        v_ref = dataset.version
        v_ref.major = 24
        self.assertEqual(dataset.version, v_ref)
        self.assertNotEqual(dataset, dataset_2)
        # Check assignment
        dataset.version = v
        self.assertEqual(dataset.version, v_ref)
        self.assertEqual(dataset.version, v)
        self.assertEqual(dataset, dataset_2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
