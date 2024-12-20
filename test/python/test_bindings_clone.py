import numpy as np
import gc
from dataset_gen import gen_fake_dataset


def generic_clone_test(self, var, copy_ctor, clone):
    var_ref = var
    var_2 = copy_ctor(var)
    var_cloned = clone(var)
    self.assertEqual(var, var_ref)
    self.assertEqual(var, var_2)
    self.assertEqual(var, var_cloned)
    self.assertEqual(hex(id(var)), hex(id(var_ref)))
    self.assertNotEqual(hex(id(var)), hex(id(var_2)))
    self.assertNotEqual(hex(id(var)), hex(id(var_cloned)))


def test_clone_version(
    self,
    version_args_constructor,
    version_copy,
    clone,
):
    testName = "Clone Version binding"
    print("\n--Test %s BEGIN--" % testName)

    vers = version_args_constructor(1, 2, 3)
    generic_clone_test(self, vers, version_copy, clone)

    print("--Test %s END--" % testName)


def test_clone_vector_2D(
    self,
    vector2D_args_constructor,
    vector2D_copy,
    clone,
):
    testName = "Clone Vector2D binding"
    print("\n--Test %s BEGIN--" % testName)

    vec2D = vector2D_args_constructor(1, 2)
    generic_clone_test(self, vec2D, vector2D_copy, clone)

    print("--Test %s END--" % testName)


def test_clone_vector_3D(
    self,
    vector3D_args_constructor,
    vector3D_copy,
    clone,
):
    testName = "Clone Vector3D binding"
    print("\n--Test %s BEGIN--" % testName)

    vec3D = vector3D_args_constructor(1, 2, 3)
    generic_clone_test(self, vec3D, vector3D_copy, clone)

    print("--Test %s END--" % testName)


def test_clone_transform(
    self,
    transform_args_constructor,
    transform_copy,
    vector3D_args_constructor,
    clone,
):
    testName = "Clone Transform binding"
    print("\n--Test %s BEGIN--" % testName)

    transf = transform_args_constructor(
        vector3D_args_constructor(1, 2, 3), vector3D_args_constructor(4, 5, 6)
    )
    generic_clone_test(self, transf, transform_copy, clone)

    print("--Test %s END--" % testName)


def test_clone_wave(
    self,
    wave_constructor,
    wave_copy,
    vector3D_args_constructor,
    enum_wave,
    clone,
):
    testName = "Clone Wave binding"
    print("\n--Test %s BEGIN--" % testName)

    w = wave_constructor()
    w.type = enum_wave().DIVERGING_WAVE
    w.time_zero = 42
    w.time_zero_reference_point = vector3D_args_constructor(7, 8, 9)
    w.parameters = [12.34, 56.7]

    generic_clone_test(self, w, wave_copy, clone)

    print("--Test %s END--" % testName)


def test_clone_impulse_response(
    self,
    impulse_response_constructor,
    impulse_response_copy,
    clone,
):
    testName = "Clone ImpulseResponse binding"
    print("\n--Test %s BEGIN--" % testName)

    ir = impulse_response_constructor()
    ir.sampling_frequency = 120e6
    ir.time_offset = 100e-6
    ir.units = "azerty"
    ir.data = [12.34, 56.7, 7.89e10]

    generic_clone_test(self, ir, impulse_response_copy, clone)

    print("--Test %s END--" % testName)


def test_clone_element_geometry(
    self,
    element_geometry_constructor,
    element_geometry_copy,
    vector3D_args_constructor,
    clone,
):
    testName = "Clone ElementGeometry binding"
    print("\n--Test %s BEGIN--" % testName)

    eg = element_geometry_constructor()
    eg.perimeter.append(vector3D_args_constructor(0, 0, 0))
    eg.perimeter.append(vector3D_args_constructor(0, 1, 0))
    eg.perimeter.append(vector3D_args_constructor(1, 1, 0))
    eg.perimeter.append(vector3D_args_constructor(1, 0, 0))

    generic_clone_test(self, eg, element_geometry_copy, clone)

    print("--Test %s END--" % testName)


def test_clone_element(
    self,
    element_constructor,
    element_copy,
    element_geometry_constructor,
    impulse_response_constructor,
    transform_args_constructor,
    vector3D_args_constructor,
    clone,
):
    testName = "Clone Element binding"
    print("\n--Test %s BEGIN--" % testName)

    transf = transform_args_constructor(
        vector3D_args_constructor(1, 2, 3), vector3D_args_constructor(4, 5, 6)
    )

    eg = element_geometry_constructor()
    eg.perimeter.append(vector3D_args_constructor(0, 0, 0))
    eg.perimeter.append(vector3D_args_constructor(0, 1, 0))
    eg.perimeter.append(vector3D_args_constructor(1, 1, 0))
    eg.perimeter.append(vector3D_args_constructor(1, 0, 0))

    ir = impulse_response_constructor()
    ir.sampling_frequency = 120e6

    elt = element_constructor()
    elt.transform = transf
    elt.element_geometry = eg
    elt.impulse_response = ir

    generic_clone_test(self, elt, element_copy, clone)

    elt_ref = elt
    elt_2 = element_copy(elt)
    elt_cloned = clone(elt)

    self.assertEqual(hex(id(elt.element_geometry)), hex(id(elt_ref.element_geometry)))
    self.assertEqual(hex(id(elt.element_geometry)), hex(id(elt_2.element_geometry)))
    self.assertEqual(hex(id(elt.element_geometry)), hex(id(elt_cloned.element_geometry)))

    self.assertEqual(hex(id(elt.impulse_response)), hex(id(elt_ref.impulse_response)))
    self.assertEqual(hex(id(elt.impulse_response)), hex(id(elt_2.impulse_response)))
    self.assertEqual(hex(id(elt.impulse_response)), hex(id(elt_cloned.impulse_response)))

    print("--Test %s END--" % testName)


def test_clone_excitation(
    self,
    excitation_constructor,
    excitation_copy,
    clone,
):
    testName = "Clone Element binding"
    print("\n--Test %s BEGIN--" % testName)

    ex = excitation_constructor()
    ex.pulse_shape = "pulse shape"
    ex.transmit_frequency = 10e6
    ex.waveform = np.arange(100, dtype=np.float64)

    generic_clone_test(self, ex, excitation_copy, clone)

    print("--Test %s END--" % testName)


def test_clone_transmit_setup(
    self,
    transmit_setup_constructor,
    transmit_setup_copy,
    probe_constructor,
    excitation_constructor,
    transform_args_constructor,
    vector3D_args_constructor,
    enum_wave,
    clone,
):
    testName = "Clone TransmitSetup binding"
    print("\n--Test %s BEGIN--" % testName)

    probe = probe_constructor()
    probe.description = "undefined probe"

    ex = excitation_constructor()
    ex.transmit_frequency = 10e6

    ex2 = excitation_constructor()
    ex2.pulse_shape = "pulse shape"

    ts = transmit_setup_constructor()
    ts.probe = probe
    ts.probe_transform = transform_args_constructor(
        vector3D_args_constructor(5.1e-120, -8, 7.0), vector3D_args_constructor(5.2, 4.3, 8.2e10)
    )
    ts.time_offset = 12052
    ts.wave.type = enum_wave().CONVERGING_WAVE
    ts.wave.time_zero_reference_point = vector3D_args_constructor(1, 2, 3)
    ts.wave.time_zero = 4.11
    ts.wave.parameters = [7, 0.3, 5.6, 7]
    ts.active_elements = [[1], [0, 1]]
    ts.excitations = [
        ex,
        ex2,
    ]
    ts.delays = [0.3, 1.2]

    generic_clone_test(self, ts, transmit_setup_copy, clone)

    ts_ref = ts
    ts_2 = transmit_setup_copy(ts)
    ts_cloned = clone(ts)

    for i in range(len(ts.excitations)):
        self.assertEqual(hex(id(ts.excitations[i])), hex(id(ts_ref.excitations[i])))
        self.assertEqual(hex(id(ts.excitations[i])), hex(id(ts_2.excitations[i])))
        self.assertEqual(hex(id(ts.excitations[i])), hex(id(ts_cloned.excitations[i])))

    self.assertEqual(hex(id(ts.probe)), hex(id(ts_ref.probe)))
    self.assertEqual(hex(id(ts.probe)), hex(id(ts_2.probe)))
    self.assertEqual(hex(id(ts.probe)), hex(id(ts_cloned.probe)))

    print("--Test %s END--" % testName)


def test_clone_receive_setup(
    self,
    receive_setup_constructor,
    receive_setup_copy,
    probe_constructor,
    transform_args_constructor,
    vector3D_args_constructor,
    clone,
):
    testName = "Clone ReceiveSetup binding"
    print("\n--Test %s BEGIN--" % testName)

    probe = probe_constructor()
    probe.description = "undefined probe"

    rs = receive_setup_constructor()
    rs.probe = probe
    rs.probe_transform = transform_args_constructor(
        vector3D_args_constructor(5.1e-120, -8, 7.0), vector3D_args_constructor(5.2, 4.3, 8.2e10)
    )
    rs.sampling_frequency = 1268
    rs.number_samples = 42
    rs.active_elements = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
    rs.tgc_profile = [1, 0.2, 4.0, 5, 0.1, 5, 0.45, 5]
    rs.tgc_sampling_frequency = 4.5
    rs.modulation_frequency = 4.58
    rs.time_offset = 7.8

    generic_clone_test(self, rs, receive_setup_copy, clone)

    rs_ref = rs
    rs_2 = receive_setup_copy(rs)
    rs_cloned = clone(rs)

    self.assertEqual(hex(id(rs.probe)), hex(id(rs_ref.probe)))
    self.assertEqual(hex(id(rs.probe)), hex(id(rs_2.probe)))
    self.assertEqual(hex(id(rs.probe)), hex(id(rs_cloned.probe)))

    print("--Test %s END--" % testName)


def test_clone_event(
    self,
    event_constructor,
    event_copy,
    clone,
):
    testName = "Clone Event binding"
    print("\n--Test %s BEGIN--" % testName)

    event = event_constructor()
    generic_clone_test(self, event, event_copy, clone)

    print("--Test %s END--" % testName)


def test_clone_group(
    self,
    group_constructor,
    group_copy,
    event_constructor,
    enum_sampling,
    enum_data,
    clone,
):
    testName = "Clone Group binding"
    print("\n--Test %s BEGIN--" % testName)

    evt = event_constructor()
    evt.receive_setup.sampling_frequency = 120e6
    evt2 = event_constructor()
    evt2.receive_setup.sampling_frequency = 100e6

    group = group_constructor()
    group.sampling_type = enum_sampling().IQ
    group.data_type = enum_data().DOUBLE
    group.description = "dummmy group"
    group.sound_speed = 1500
    group.sequence = [evt, evt2]
    generic_clone_test(self, group, group_copy, clone)

    print("--Test %s END--" % testName)


def test_clone_group_data(
    self,
    group_data_constructor,
    group_data_copy,
    group_constructor,
    enum_sampling,
    enum_data,
    clone,
):
    testName = "Clone GroupData binding"
    print("\n--Test %s BEGIN--" % testName)

    group = group_constructor()
    group.sampling_type = enum_sampling().IQ
    group.data_type = enum_data().DOUBLE

    group_data = group_data_constructor()
    group_data.group = group
    group_data.raw_data = np.array([11 + 2j, -3 + 4.5j], np.complex128)
    group_data.sequence_timestamps = [1, 2, 3, 4.56]
    group_data.event_timestamps = [[1, 2, 3, 4.56], [7.8, 9]]

    generic_clone_test(self, group_data, group_data_copy, clone)

    group_data_ref = group_data
    group_data_2 = group_data_copy(group_data)
    group_data_cloned = clone(group_data)

    self.assertEqual(hex(id(group_data.group)), hex(id(group_data_ref.group)))
    self.assertEqual(hex(id(group_data.group)), hex(id(group_data_2.group)))
    self.assertEqual(hex(id(group_data.group)), hex(id(group_data_cloned.group)))

    self.assertEqual(hex(id(group_data.raw_data)), hex(id(group_data_ref.raw_data)))
    self.assertEqual(hex(id(group_data.raw_data)), hex(id(group_data_2.raw_data)))
    self.assertEqual(hex(id(group_data.raw_data)), hex(id(group_data_cloned.raw_data)))

    print("--Test %s END--" % testName)


def test_clone_probe(
    self,
    probe_copy,
    dataset_constructor,
    probe_constructor,
    excitation_constructor,
    group_constructor,
    group_data_constructor,
    element_geometry_constructor,
    element_constructor,
    event_constructor,
    impulse_response_constructor,
    transform_args_constructor,
    vector3D_args_constructor,
    enum_probe,
    enum_sampling,
    enum_data,
    enum_wave,
    clone,
):
    testName = "Clone Probe binding"
    print("\n--Test %s BEGIN--" % testName)

    d = gen_fake_dataset(
        dataset_constructor,
        probe_constructor,
        excitation_constructor,
        group_constructor,
        group_data_constructor,
        element_geometry_constructor,
        element_constructor,
        event_constructor,
        impulse_response_constructor,
        transform_args_constructor,
        vector3D_args_constructor,
        enum_probe,
        enum_sampling,
        enum_data,
        enum_wave,
    )

    for p_id in range(len(d.acquisition.probes)):
        probe = d.acquisition.probes[p_id]
        generic_clone_test(self, probe, probe_copy, clone)

        probe_cloned = clone(probe)
        self.assertEqual(len(probe.element_geometries), len(probe_cloned.element_geometries))
        self.assertEqual(len(probe.impulse_responses), len(probe_cloned.impulse_responses))
        self.assertEqual(len(probe.elements), len(probe_cloned.elements))
        for eg_id in range(len(probe.element_geometries)):
            self.assertEqual(
                probe_cloned.element_geometries[eg_id], probe.element_geometries[eg_id]
            )

        for ir_id in range(len(probe.impulse_responses)):
            self.assertEqual(probe_cloned.impulse_responses[ir_id], probe.impulse_responses[ir_id])

        for e_id in range(len(probe.elements)):
            eg_id = -1
            for i in range(len(probe_cloned.element_geometries)):
                if (
                    probe_cloned.element_geometries[i]
                    == probe_cloned.elements[e_id].element_geometry
                ):
                    eg_id = i
                    break
            if eg_id < 0:
                self.assertEqual(probe_cloned.elements[e_id].element_geometry, None)
            else:
                self.assertEqual(
                    probe_cloned.elements[e_id].element_geometry,
                    probe_cloned.element_geometries[eg_id],
                )
                self.assertEqual(
                    probe_cloned.elements[e_id].element_geometry, probe.element_geometries[eg_id]
                )

                eg_id_bis = -1
                for i in range(len(probe.element_geometries)):
                    if probe.element_geometries[i] == probe_cloned.elements[e_id].element_geometry:
                        eg_id_bis = i
                        break
                self.assertNotEqual(-1, eg_id_bis)

            ir_id = -1
            for i in range(len(probe_cloned.impulse_responses)):
                if (
                    probe_cloned.impulse_responses[i]
                    == probe_cloned.elements[e_id].impulse_response
                ):
                    ir_id = i
                    break
            if ir_id < 0:
                self.assertEqual(probe_cloned.elements[e_id].impulse_response, None)
            else:
                self.assertEqual(
                    probe_cloned.elements[e_id].impulse_response,
                    probe_cloned.impulse_responses[ir_id],
                )
                self.assertEqual(
                    probe_cloned.elements[e_id].impulse_response, probe.impulse_responses[ir_id]
                )

                ir_id_bis = -1
                for i in range(len(probe.impulse_responses)):
                    if probe.impulse_responses[i] == probe_cloned.elements[e_id].impulse_response:
                        ir_id_bis = i
                        break
                self.assertNotEqual(-1, ir_id_bis)

    # PROBE 1

    probe_1 = d.acquisition.probes[1]
    probe_1_cloned = clone(probe_1)

    self.assertEqual(probe_1_cloned.element_geometries[0], element_geometry_constructor())
    self.assertEqual(
        probe_1_cloned.element_geometries[1].perimeter,
        [
            vector3D_args_constructor(15, 23, 543),
            vector3D_args_constructor(2.2, 1.4, 0.2),
        ],
    )
    self.assertEqual(
        probe_1_cloned.element_geometries[2].perimeter,
        [
            vector3D_args_constructor(2.2, 1.4, 0.2),
            vector3D_args_constructor(1.2, 2.4, 543),
        ],
    )

    self.assertEqual(probe_1_cloned.impulse_responses[0], impulse_response_constructor())
    self.assertEqual(probe_1_cloned.impulse_responses[1].units, "meter")
    self.assertEqual(probe_1_cloned.impulse_responses[2].units, "milli")

    self.assertEqual(probe_1_cloned.elements[0], element_constructor())

    self.assertEqual(
        probe_1_cloned.elements[1].element_geometry, probe_1_cloned.element_geometries[0]
    )
    self.assertEqual(
        probe_1_cloned.elements[1].impulse_response, probe_1_cloned.impulse_responses[1]
    )

    self.assertEqual(
        probe_1_cloned.elements[2].element_geometry, probe_1_cloned.element_geometries[1]
    )
    self.assertEqual(
        probe_1_cloned.elements[2].impulse_response, probe_1_cloned.impulse_responses[2]
    )

    self.assertEqual(
        probe_1_cloned.elements[3].element_geometry, probe_1_cloned.element_geometries[1]
    )
    self.assertEqual(
        probe_1_cloned.elements[3].impulse_response, probe_1_cloned.impulse_responses[0]
    )

    self.assertEqual(
        probe_1_cloned.elements[4].element_geometry, probe_1_cloned.element_geometries[2]
    )
    self.assertEqual(
        probe_1_cloned.elements[4].impulse_response, probe_1_cloned.impulse_responses[1]
    )

    # Change probe_1_cloned and check probe_1 is not affected
    self.assertEqual(probe_1, probe_1_cloned)

    probe_1_cloned.element_geometries[0].perimeter = [
        vector3D_args_constructor(15, 23, 543),
        vector3D_args_constructor(2.2, 1.4, 0.2),
    ]
    self.assertEqual(probe_1.element_geometries[0], element_geometry_constructor())
    self.assertEqual(
        probe_1_cloned.elements[1].element_geometry, probe_1_cloned.element_geometries[0]
    )
    self.assertNotEqual(probe_1.elements[1].element_geometry, probe_1_cloned.element_geometries[0])

    probe_1_cloned.impulse_responses[0].units = "toto"
    self.assertEqual(probe_1.impulse_responses[0], impulse_response_constructor())
    self.assertEqual(
        probe_1_cloned.elements[3].impulse_response, probe_1_cloned.impulse_responses[0]
    )
    self.assertNotEqual(probe_1.elements[3].impulse_response, probe_1_cloned.impulse_responses[0])

    self.assertNotEqual(probe_1, probe_1_cloned)

    # PROBE 2

    probe_2 = d.acquisition.probes[2]
    probe_2_cloned = clone(probe_2)

    self.assertEqual(probe_2_cloned.element_geometries[0], element_geometry_constructor())
    self.assertEqual(
        probe_2_cloned.element_geometries[1].perimeter,
        [
            vector3D_args_constructor(123, 4.5, 5.4),
            vector3D_args_constructor(1.1, 22.1, 5.4),
        ],
    )
    self.assertEqual(
        probe_2_cloned.element_geometries[2].perimeter,
        [
            vector3D_args_constructor(5.5, 6.6, 7.7),
            vector3D_args_constructor(1.1, 0.0, 5.5),
        ],
    )

    self.assertEqual(probe_2_cloned.impulse_responses[0], impulse_response_constructor())
    self.assertEqual(probe_2_cloned.impulse_responses[1].units, "met55er")
    self.assertEqual(probe_2_cloned.impulse_responses[2].units, "mi3lli")

    self.assertEqual(
        probe_2_cloned.elements[0].element_geometry, probe_2_cloned.element_geometries[1]
    )
    self.assertEqual(
        probe_2_cloned.elements[0].impulse_response, probe_2_cloned.impulse_responses[1]
    )

    self.assertEqual(
        probe_2_cloned.elements[1].element_geometry, probe_2_cloned.element_geometries[2]
    )
    self.assertEqual(
        probe_2_cloned.elements[1].impulse_response, probe_2_cloned.impulse_responses[2]
    )

    # Change probe_2_cloned and check probe_2 is not affected
    self.assertEqual(probe_2, probe_2_cloned)

    probe_2_cloned.element_geometries[0].perimeter = [
        vector3D_args_constructor(15, 23, 543),
        vector3D_args_constructor(2.2, 1.4, 0.2),
    ]
    self.assertEqual(probe_2.element_geometries[0], element_geometry_constructor())
    probe_2_cloned.element_geometries.append(element_geometry_constructor())
    self.assertEqual(len(probe_2.element_geometries) + 1, len(probe_2_cloned.element_geometries))

    probe_2_cloned.impulse_responses[0].units = "toto"
    self.assertEqual(probe_2.impulse_responses[0], impulse_response_constructor())
    probe_2_cloned.impulse_responses.append(impulse_response_constructor())
    self.assertEqual(len(probe_2.impulse_responses) + 1, len(probe_2_cloned.impulse_responses))

    self.assertNotEqual(probe_2, probe_2_cloned)

    print("--Test %s END--" % testName)


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
    enum_data,
    enum_wave,
    clone,
):
    testName = "Clone binding"
    print("\n--Test %s BEGIN--" % testName)

    fake_dataset = gen_fake_dataset(
        dataset_constructor,
        probe_constructor,
        excitation_constructor,
        group_constructor,
        group_data_constructor,
        element_geometry_constructor,
        element_constructor,
        event_constructor,
        impulse_response_constructor,
        transform_args_constructor,
        vector3D_args_constructor,
        enum_probe,
        enum_sampling,
        enum_data,
        enum_wave,
    )

    # acq = acq_constructor()

    # # Check copy CTOR and referencing object
    # acq_2 = acq_copy(acq)
    # self.assertEqual(acq, acq_2)
    # self.assertNotEqual(hex(id(acq), hex(id(acq_2))
    # acq_2.authors = "Hello"
    # self.assertNotEqual(acq, acq_2)
    # acq_ref = acq
    # acq_ref.authors = "Hello"
    # self.assertEqual(acq, acq_ref)
    # self.assertEqual(hex(id(acq), hex(id(acq_ref))

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
