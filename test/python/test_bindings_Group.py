import pyurx as urx
import unittest


class TestBindingsGroup(unittest.TestCase):

    def testGroup(self):
        testName = "Group binding"
        print("\n--Test %s BEGIN--" % testName)

        # Check default CTOR
        g = urx.Group()
        self.assertEqual(g.sampling_type, urx.SamplingType.UNDEFINED)
        self.assertEqual(g.data_type, urx.DataType.UNDEFINED)
        self.assertEqual(g.description, "")
        self.assertEqual(g.sequence, urx.VecEvent())

        # Check copy CTOR and referencing object
        g_2 = urx.Group(g)
        self.assertEqual(g, g_2)
        g_2.description = "Hello"
        self.assertNotEqual(g, g_2)
        g_ref = g
        g_ref.description = "Hello"
        self.assertEqual(g, g_ref)

        rs = urx.ReceiveSetup(urx.Probe(), urx.Transform(),
                              1, 2, [[3]], [4], 5, 6, 7)
        ts = urx.TransmitSetup(urx.Probe(), urx.Wave(), urx.Transform(), 42)
        evt = urx.Event()
        evt_2 = urx.Event(ts, rs)

        # Check CTOR with all parameters
        g = urx.Group(urx.SamplingType.RF, urx.DataType.INT32,
                      "BMode", [evt, evt_2])
        self.assertEqual(g.sampling_type, urx.SamplingType.RF)
        self.assertEqual(g.data_type, urx.DataType.INT32)
        self.assertEqual(g.description, "BMode")
        self.assertEqual(g.sequence, [evt, evt_2])
        g_2 = urx.Group(g)

        # Reference is not possible for enum
        self.assertEqual(g.sampling_type, urx.SamplingType.RF)
        sampling_type_2 = g.sampling_type
        sampling_type_2 = urx.SamplingType.IQ
        self.assertNotEqual(g.sampling_type, sampling_type_2)
        self.assertEqual(g, g_2)
        # Check assignment
        g.sampling_type = urx.SamplingType.RF
        self.assertEqual(g.sampling_type, urx.SamplingType.RF)
        self.assertEqual(g, g_2)

        # Reference is not possible for enum
        self.assertEqual(g.data_type, urx.DataType.INT32)
        data_type_2 = g.data_type
        data_type_2 = urx.DataType.DOUBLE
        self.assertNotEqual(g.data_type, data_type_2)
        self.assertEqual(g, g_2)
        # Check assignment
        g.data_type = urx.DataType.INT32
        self.assertEqual(g.data_type, urx.DataType.INT32)
        self.assertEqual(g, g_2)

        # Reference is not possible for string
        self.assertEqual(g.description, "BMode")
        description_2 = g.description
        description_2 = "PW"
        self.assertNotEqual(g.description, description_2)
        self.assertEqual(g, g_2)
        # Check assignment
        g.description = "BMode"
        self.assertEqual(g.description, "BMode")
        self.assertEqual(g, g_2)

        # Reference is possible for sequence (VecEvent)
        self.assertEqual(g.sequence, [evt, evt_2])
        seq_ref = g.sequence
        seq_ref.append(evt_2)
        self.assertEqual(g.sequence, seq_ref)
        self.assertNotEqual(g, g_2)
        # Check assignment
        g.sequence = [evt, evt_2]
        self.assertEqual(g.sequence, seq_ref)
        self.assertEqual(g.sequence, [evt, evt_2])
        self.assertEqual(g, g_2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
