import os

import ultrasound_rawdata_exchange as urx
import unittest


class TestBindingsIo(unittest.TestCase):

    def test_io(self):
        if hasattr(urx, "loadFromFile"):
            with self.assertRaises(urx.ReadFileException):
                urx.loadFromFile("missing_file.urx")
            dataset = urx.Dataset()
            with self.assertRaises(urx.WriteFileException):
                if os.name == "nt":
                    urx.saveToFile("aux", dataset)
                else:
                    urx.saveToFile("/", dataset)


if __name__ == "__main__":
    unittest.main()
