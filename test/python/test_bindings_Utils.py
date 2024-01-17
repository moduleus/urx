import pyurx as urx
import unittest
import gc
from pathlib import Path


class TestBindingsUtils(unittest.TestCase):

    def testIO(self):
        testName = "Utils IO binding"
        print("\n--Test %s BEGIN--" % testName)

        t = urx.load_from_file(str((Path(__file__).parent.absolute(
        ) / "../io/resources/v_0_3_channel_mapping.uff")))

        urx.save_to_file(str((Path(__file__).parent.absolute(
        ) / "../io/resources/v_0_3_channel_mapping.uff.pyres")), t)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
