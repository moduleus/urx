import pyurx as urx
import unittest


class TestBindingsEvent(unittest.TestCase):

    def testEvent(self):
        testName = "Event"
        print("\n--Test %s binding BEGIN--" % testName)

        # Check default CTOR
        evt = urx.Event()
        self.assertEqual(evt.transmit_setup, urx.TransmitSetup())
        self.assertEqual(evt.receive_setup, urx.ReceiveSetup())

        rs = urx.ReceiveSetup(urx.Probe(), urx.Transform(),
                              1, 2, [[3]], [4], 5, 6, 7)
        ts = urx.TransmitSetup(urx.Probe(), urx.Wave(), urx.Transform(), 42)

        # Check copy CTOR and referencing object
        evt_2 = urx.Event(evt)
        self.assertEqual(evt, evt_2)
        evt_2.receive_setup = rs
        self.assertNotEqual(evt, evt_2)
        evt_ref = evt
        evt_ref.receive_setup = rs
        self.assertEqual(evt, evt_ref)

        # Check CTOR with all parameters
        evt = urx.Event(ts, rs)
        self.assertEqual(evt.transmit_setup, ts)
        self.assertEqual(evt.receive_setup, rs)
        evt_2 = urx.Event(evt)

        # Reference is possible for transmit_setup (TransmitSetup)
        self.assertEqual(evt.transmit_setup, ts)
        ts_ref = evt.transmit_setup
        probe_2 = urx.Probe()
        probe_2.description = 'rca'
        ts_ref.probe = probe_2
        self.assertEqual(evt.transmit_setup, ts_ref)
        self.assertNotEqual(evt, evt_2)
        # Check assignment
        evt.transmit_setup = ts
        self.assertEqual(evt.transmit_setup, ts_ref)
        self.assertEqual(evt.transmit_setup, ts)
        self.assertEqual(evt, evt_2)

        # Reference is possible for receive_setup (ReceiveSetup)
        self.assertEqual(evt.receive_setup, rs)
        rs_ref = evt.receive_setup
        rs_ref.probe = probe_2
        self.assertEqual(evt.receive_setup, rs_ref)
        self.assertNotEqual(evt, evt_2)
        # Check assignment
        evt.receive_setup = rs
        self.assertEqual(evt.receive_setup, rs_ref)
        self.assertEqual(evt.receive_setup, rs)
        self.assertEqual(evt, evt_2)

        print("--Test %s END--" % testName)


if __name__ == '__main__':
    unittest.main()
