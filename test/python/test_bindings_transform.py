import numpy as np


def test_transform(
    self,
    transform_constructor,
    transform_copy,
    transform_args,
    vector3d_constructor,
    vector3d_args,
):
    testName = "Transform binding"
    print("\n--Test %s BEGIN--" % testName)

    tf = transform_constructor()

    self.assertEqual(tf.rotation, vector3d_constructor())
    self.assertEqual(tf.translation, vector3d_constructor())
    r = vector3d_args(1, 2, -3.14)
    t = vector3d_args(4, 5, -6.78)

    tf = transform_args(r, t)
    self.assertEqual(tf.rotation, r)
    self.assertEqual(tf.translation, t)

    tf_2 = transform_copy(tf)
    self.assertEqual(tf, tf_2)

    r_2 = r
    r.x = 42
    self.assertNotEqual(tf.rotation, r)
    self.assertNotEqual(tf.rotation, r_2)

    v = vector3d_constructor()
    v.x = 1.0
    v.y = 2.0
    v.z = 3.0
    tf.rotation = v
    self.assertEqual(tf.rotation.x, v.x)
    self.assertEqual(tf.rotation.y, v.y)
    self.assertEqual(tf.rotation.z, v.z)
    tf.rotation = np.array([2.0, 3.0, 4.0])
    self.assertEqual(tf.rotation.x, 2.0)
    self.assertEqual(tf.rotation.y, 3.0)
    self.assertEqual(tf.rotation.z, 4.0)

    print("--Test %s END--" % testName)
