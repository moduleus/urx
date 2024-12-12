import numpy as np
import gc


def test_clone(
    self,
    dataset_constructor,
    acq_constructor,
    acq_copy,
    double_nan_constructor,
    double_nan_copy,
    probe_constructor,
    probe_copy,
    excitation_constructor,
    excitation_copy,
    group_constructor,
    group_copy,
    group_data_constructor,
    group_data_copy,
    element_geometry_constructor,
    element_constructor,
    element_copy,
    event_constructor,
    impulse_response_constructor,
    receive_setup_constructor,
    transmit_setup_constructor,
    wave_constructor,
    transform_args_constructor,
    vector2D_args_constructor,
    vector3D_args_constructor,
    version_args_constructor,
    enum_probe,
    enum_sampling,
    enum_wave,
    clone,
):
    testName = "Clone binding"
    print("\n--Test %s BEGIN--" % testName)

    # Version
    vers = version_args_constructor(1, 2, 3)
    vers_ref = vers
    vers_cloned = clone(vers)
    self.assertEqual(vers, vers_ref)
    self.assertEqual(vers, vers_cloned)
    self.assertEqual(id(vers), id(vers_ref))
    self.assertNotEqual(id(vers), id(vers_cloned))

    # Vector2D
    vec2D = vector2D_args_constructor(1, 2)
    vec2D_ref = vec2D
    vec2D_cloned = clone(vec2D)
    self.assertEqual(vec2D, vec2D_ref)
    self.assertEqual(vec2D, vec2D_cloned)
    self.assertEqual(id(vec2D), id(vec2D_ref))
    self.assertNotEqual(id(vec2D), id(vec2D_cloned))

    # Vector3D
    vec3D = vector3D_args_constructor(1, 2, 3)
    vec3D_ref = vec3D
    vec3D_cloned = clone(vec3D)
    self.assertEqual(vec3D, vec3D_ref)
    self.assertEqual(vec3D, vec3D_cloned)
    self.assertEqual(id(vec3D), id(vec3D_ref))
    self.assertNotEqual(id(vec3D), id(vec3D_cloned))

    # Transform
    transf = transform_args_constructor(
        vector3D_args_constructor(1, 2, 3), vector3D_args_constructor(4, 5, 6)
    )
    transf_ref = transf
    transf_cloned = clone(transf)
    self.assertEqual(transf, transf_ref)
    self.assertEqual(transf, transf_cloned)
    self.assertEqual(id(transf), id(transf_ref))
    self.assertNotEqual(id(transf), id(transf_cloned))

    # Wave
    w = wave_constructor()
    w.type = enum_wave().DIVERGING_WAVE
    w.time_zero = 42
    w.time_zero_reference_point = vector3D_args_constructor(7, 8, 9)
    w.parameters = [12.34, 56.7]
    w_ref = w
    w_cloned = clone(w)
    self.assertEqual(w, w_ref)
    self.assertEqual(w, w_cloned)
    self.assertEqual(id(w), id(w_ref))
    self.assertNotEqual(id(w), id(w_cloned))

    # Impulse Response
    ir = impulse_response_constructor()
    ir.sampling_frequency = 120e6
    ir.time_offset = 100e-6
    ir.units = "azerty"
    ir.data = [12.34, 56.7, 7.89e10]
    ir_ref = ir
    ir_cloned = clone(ir)
    self.assertEqual(ir, ir_ref)
    self.assertEqual(ir, ir_cloned)
    self.assertEqual(id(ir), id(ir_ref))
    self.assertNotEqual(id(ir), id(ir_cloned))

    # Element Geometry
    eg = element_geometry_constructor()
    eg.perimeter.append(vector3D_args_constructor(0, 0, 0))
    eg.perimeter.append(vector3D_args_constructor(0, 1, 0))
    eg.perimeter.append(vector3D_args_constructor(1, 1, 0))
    eg.perimeter.append(vector3D_args_constructor(1, 0, 0))
    eg_ref = eg
    eg_cloned = clone(eg)
    self.assertEqual(eg, eg_ref)
    self.assertEqual(eg, eg_cloned)
    self.assertEqual(id(eg), id(eg_ref))
    self.assertNotEqual(id(eg), id(eg_cloned))

    # Element
    elt = element_constructor()
    elt.transform = transf
    elt.element_geometry = eg_cloned
    elt.impulse_response = ir_cloned
    elt_ref = elt
    elt_2 = element_copy(elt)
    elt_cloned = clone(elt)

    self.assertEqual(elt, elt_ref)
    self.assertEqual(elt, elt_2)
    self.assertEqual(elt, elt_cloned)
    self.assertEqual(id(elt), id(elt_ref))
    self.assertNotEqual(id(elt), id(elt_2))
    self.assertNotEqual(id(elt), id(elt_cloned))

    self.assertEqual(id(elt.element_geometry), id(elt_ref.element_geometry))
    self.assertEqual(id(elt.element_geometry), id(elt_2.element_geometry))
    self.assertEqual(id(elt.element_geometry), id(elt_cloned.element_geometry))

    self.assertEqual(id(elt.impulse_response), id(elt_ref.impulse_response))
    self.assertEqual(id(elt.impulse_response), id(elt_2.impulse_response))
    self.assertEqual(id(elt.impulse_response), id(elt_cloned.impulse_response))

    # Excitation Geometry
    ex = excitation_constructor()
    ex.pulse_shape = "pulse shape"
    ex.transmit_frequency = 10e6
    # ex.waveform = [range(0, 100)]
    ex_ref = ex
    ex_2 = excitation_copy(ex)
    ex_cloned = clone(ex)
    self.assertEqual(ex, ex_ref)
    self.assertEqual(ex, ex_2)
    self.assertEqual(ex, ex_cloned)
    self.assertEqual(id(ex), id(ex_ref))
    self.assertNotEqual(id(ex), id(ex_2))
    self.assertNotEqual(id(ex), id(ex_cloned))

    # acq = acq_constructor()

    # # Check copy CTOR and referencing object
    # acq_2 = acq_copy(acq)
    # self.assertEqual(acq, acq_2)
    # self.assertNotEqual(id(acq), id(acq_2))
    # acq_2.authors = "Hello"
    # self.assertNotEqual(acq, acq_2)
    # acq_ref = acq
    # acq_ref.authors = "Hello"
    # self.assertEqual(acq, acq_ref)
    # self.assertEqual(id(acq), id(acq_ref))

    # acq = acq_constructor()
    # acq_2 = acq_copy(acq)

    # # probes is a pointer vector, thus all modifications are shared
    # p = probe_constructor()
    # p_rca = probe_constructor()
    # p_rca.type = enum_probe().RCA
    # p_rca_bis = probe_copy(p_rca)
    # acq.probes = [p, p_rca]
    # acq_2 = acq_copy(acq)
    # self.assertEqual(acq.probes[1], p_rca)
    # self.assertEqual(acq, acq_2)

    # p_rca.type = enum_probe().LINEAR
    # self.assertEqual(acq.probes[1], p_rca)
    # self.assertEqual(acq_2.probes[1], p_rca)
    # p_rca.type = enum_probe().RCA

    # # Reference is possible for shared pointers vector
    # self.assertEqual(acq.probes, [p, p_rca])
    # probes_ref = acq.probes
    # probes_ref[0] = p_rca_bis
    # self.assertEqual(acq.probes, probes_ref)
    # self.assertNotEqual(acq, acq_2)
    # # Check assignment
    # acq.probes = [p, p_rca]
    # self.assertEqual(acq.probes, probes_ref)
    # self.assertEqual(acq.probes, [p, p_rca])
    # self.assertEqual(acq, acq_2)

    # # deleting the local variable does not impact the shared pointers vectors
    # del p
    # del p_rca
    # gc.collect()
    # self.assertEqual(len(acq.probes), 2)
    # self.assertEqual(len(acq_2.probes), 2)

    # # deleting the pointers inside the vectors to check resize
    # del acq.probes[0]
    # del acq_2.probes[0]
    # gc.collect()
    # self.assertEqual(len(acq.probes), 1)
    # self.assertEqual(len(acq_2.probes), 1)
    # self.assertRaises(IndexError, lambda acq: acq.probes[1], acq)
    # self.assertEqual(acq.probes[0].type, enum_probe().RCA)

    # # excitations is a pointer vector, thus all modifications are shared
    # ex = excitation_constructor()
    # ex_2 = excitation_constructor()
    # ex_2.pulse_shape = "Hello"
    # ex_3 = excitation_copy(ex_2)
    # acq.excitations = [ex, ex_2]
    # acq_2 = acq_copy(acq)
    # self.assertEqual(acq.excitations[1], ex_2)
    # self.assertEqual(acq, acq_2)

    # ex_2.pulse_shape = "World"
    # self.assertEqual(acq.excitations[1], ex_2)
    # self.assertEqual(acq_2.excitations[1], ex_2)
    # ex_2.pulse_shape = "Hello"

    # # Reference is possible for shared pointers vector
    # self.assertEqual(acq.excitations, [ex, ex_2])
    # excitations_ref = acq.excitations
    # excitations_ref[0] = ex_3
    # self.assertEqual(acq.excitations, excitations_ref)
    # self.assertNotEqual(acq, acq_2)
    # # Check assignment
    # acq.excitations = [ex, ex_2]
    # self.assertEqual(acq.excitations, excitations_ref)
    # self.assertEqual(acq.excitations, [ex, ex_2])
    # self.assertEqual(acq, acq_2)

    # # deleting the local variable does not impact the shared pointers vectors
    # del ex
    # del ex_2
    # gc.collect()
    # self.assertEqual(len(acq.excitations), 2)
    # self.assertEqual(len(acq_2.excitations), 2)

    # # deleting the pointers inside the vectors to check resize
    # del acq.excitations[0]
    # del acq_2.excitations[0]
    # gc.collect()
    # self.assertEqual(len(acq.excitations), 1)
    # self.assertEqual(len(acq_2.excitations), 1)
    # self.assertRaises(IndexError, lambda acq: acq.excitations[1], acq)
    # self.assertEqual(acq.excitations[0].pulse_shape, "Hello")

    # # groups is a pointer vector, thus all modifications are shared
    # g = group_constructor()
    # g_2 = group_constructor()
    # g_2.sampling_type = enum_sampling().RF
    # g_3 = group_copy(g_2)
    # acq.groups = [g, g_2]
    # acq_2 = acq_copy(acq)
    # self.assertEqual(acq.groups[1], g_2)
    # self.assertEqual(acq, acq_2)

    # g_2.sampling_type = enum_sampling().IQ
    # self.assertEqual(acq.groups[1], g_2)
    # self.assertEqual(acq_2.groups[1], g_2)
    # g_2.sampling_type = enum_sampling().RF

    # # Reference is possible for shared pointers vector
    # self.assertEqual(acq.groups, [g, g_2])
    # groups_ref = acq.groups
    # groups_ref[0] = g_3
    # self.assertEqual(acq.groups, groups_ref)
    # self.assertNotEqual(acq, acq_2)
    # # Check assignment
    # acq.groups = [g, g_2]
    # self.assertEqual(acq.groups, groups_ref)
    # self.assertEqual(acq.groups, [g, g_2])
    # self.assertEqual(acq, acq_2)

    # # deleting the local variable does not impact the shared pointers vectors
    # del g
    # del g_2
    # gc.collect()
    # self.assertEqual(len(acq.groups), 2)
    # self.assertEqual(len(acq_2.groups), 2)

    # # deleting the pointers inside the vectors to check resize
    # del acq.groups[0]
    # del acq_2.groups[0]
    # gc.collect()
    # self.assertEqual(len(acq.groups), 1)
    # self.assertEqual(len(acq_2.groups), 1)
    # self.assertRaises(IndexError, lambda acq: acq.groups[1], acq)
    # self.assertEqual(acq.groups[0].sampling_type, enum_sampling().RF)

    # # groups_data is a pointer vector, thus all modifications are shared
    # if group_data_constructor is not None and group_data_copy is not None:
    #     gd = group_data_constructor()
    #     gd_2 = group_data_constructor()
    #     gd_2.group_timestamp = double_nan_copy(42)
    #     gd_3 = group_data_copy(gd_2)
    #     gd_3.group_timestamp = double_nan_copy(24)
    #     acq.groups_data = [gd, gd_2]
    #     acq_2 = acq_copy(acq)
    #     self.assertEqual(acq.groups_data[1], gd_2)
    #     self.assertEqual(acq, acq_2)

    #     # gd_2 is a copy. Not a shared_ptr.
    #     # gd_2.group_timestamp = double_nan_copy(99)
    #     # self.assertEqual(acq.groups_data[1], gd_2)
    #     # self.assertEqual(acq_2.groups_data[1], gd_2)
    #     # gd_2.group_timestamp = double_nan_copy(42)

    #     # Reference is possible for GroupData vector
    #     self.assertEqual(acq.groups_data, [gd, gd_2])
    #     groups_data_ref = acq.groups_data
    #     groups_data_ref[0] = gd_3
    #     self.assertEqual(acq.groups_data, groups_data_ref)
    #     self.assertNotEqual(acq, acq_2)
    #     # Check assignment
    #     acq.groups_data = [gd, gd_2]
    #     self.assertEqual(acq.groups_data, groups_data_ref)
    #     self.assertEqual(acq.groups_data, [gd, gd_2])
    #     self.assertEqual(acq, acq_2)

    #     # deleting the local variable does not impact the shared pointers vectors
    #     del gd
    #     del gd_2
    #     gc.collect()
    #     self.assertEqual(len(acq.groups_data), 2)
    #     self.assertEqual(len(acq_2.groups_data), 2)

    #     # deleting the pointers inside the vectors to check resize
    #     del acq.groups_data[0]
    #     del acq_2.groups_data[0]
    #     gc.collect()
    #     self.assertEqual(len(acq.groups_data), 1)
    #     self.assertEqual(len(acq_2.groups_data), 1)
    #     self.assertRaises(IndexError, lambda acq: acq.groups_data[1], acq)
    #     self.assertEqual(acq.groups_data[0].group_timestamp, double_nan_copy(42))

    print("--Test %s END--" % testName)
