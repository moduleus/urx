import pyurx as urx
import unittest
from pathlib import Path


class TestBindingsUtils(unittest.TestCase):

    def testUtilsIo(self):
        testName = "Utils IO binding"
        print("\n--Test %s BEGIN--" % testName)

        # dataset = urx.loadFromFile(str((Path(__file__).parent.absolute(
        # ) / "../io/resources/v_0_3_channel_mapping.uff")))

        # urx.saveToFile(str((Path(__file__).parent.absolute(
        # ) / "../io/resources/v_0_3_channel_mapping.uff.pyres")), dataset)

        # dataset2 = urx.loadFromFile(str((Path(__file__).parent.absolute(
        # ) / "../io/resources/v_0_3_channel_mapping.uff.pyres")))

        # self.assertEqual(dataset, dataset2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
