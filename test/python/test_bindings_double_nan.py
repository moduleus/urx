import io
import sys
import numpy as np


def test_double_nan(
    self,
    double_nan_constructor,
    double_nan_copy,
):
    testName = "DoubleNan"
    print("\n--Test %s binding BEGIN--" % testName)

    # Check CTORs
    d = double_nan_constructor()
    d_2 = double_nan_copy(d)
    d_3 = double_nan_copy(42)

    # Check Nan comparison
    self.assertTrue(np.isnan(d.value))
    self.assertEqual(d, d_2)
    self.assertEqual(d, double_nan_copy(np.nan))
    self.assertNotEqual(d, np.nan)
    self.assertNotEqual(d.value, np.nan)

    # Check non Nan comparison
    d.value = 42
    self.assertEqual(d, d_3)

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

    self.assertEqual(2.13 + d, double_nan_copy(44.13))
    self.assertEqual(d + 2.13, double_nan_copy(44.13))
    self.assertEqual(2 + d, double_nan_copy(44))
    self.assertEqual(d + 2, double_nan_copy(44))

    self.assertEqual(2.13 - d, double_nan_copy(2.13 - 42))
    self.assertEqual(d - 2.13, double_nan_copy(42 - 2.13))
    self.assertEqual(2 - d, double_nan_copy(2 - 42))
    self.assertEqual(d - 2, double_nan_copy(42 - 2))

    self.assertEqual(2.13 * d, double_nan_copy(2.13 * 42))
    self.assertEqual(d * 2.13, double_nan_copy(42 * 2.13))
    self.assertEqual(2 * d, double_nan_copy(2 * 42))
    self.assertEqual(d * 2, double_nan_copy(42 * 2))

    self.assertEqual(2.13 / d, double_nan_copy(2.13 / 42))
    self.assertEqual(d / 2.13, double_nan_copy(42 / 2.13))
    self.assertEqual(2 / d, double_nan_copy(2 / 42))
    self.assertEqual(d / 2, double_nan_copy(42 / 2))

    # Check auto convertion
    self.assertEqual(np.sin(d_3), np.sin(42))
    self.assertEqual(np.sin(d_3.value), np.sin(42))

    # Check __repr__
    capturedOutput = io.StringIO()
    sys.stdout = capturedOutput
    print(d_3, end="")
    sys.stdout = sys.__stdout__
    self.assertEqual(capturedOutput.getvalue(), "42")

    print("--Test %s END--" % testName)
