import numpy as np
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
    test_name = "Clone Version binding"
    print("\n--Test %s BEGIN--" % test_name)

    vers = version_args_constructor(1, 2, 3)
    generic_clone_test(self, vers, version_copy, clone)

    print("--Test %s END--" % test_name)


def test_clone_vector2d(
    self,
    vector2d_args_constructor,
    vector2d_copy,
    clone,
):
    test_name = "Clone Vector2D binding"
    print("\n--Test %s BEGIN--" % test_name)

    vec2d = vector2d_args_constructor(1, 2)
    generic_clone_test(self, vec2d, vector2d_copy, clone)

    print("--Test %s END--" % test_name)


def test_clone_vector3d(
    self,
    vector3d_args_constructor,
    vector3d_copy,
    clone,
):
    test_name = "Clone Vector3D binding"
    print("\n--Test %s BEGIN--" % test_name)

    vec3d = vector3d_args_constructor(1, 2, 3)
    generic_clone_test(self, vec3d, vector3d_copy, clone)

    print("--Test %s END--" % test_name)


def test_clone_transform(
    self,
    transform_args_constructor,
    transform_copy,
    vector3d_args_constructor,
    clone,
):
    test_name = "Clone Transform binding"
    print("\n--Test %s BEGIN--" % test_name)

    transf = transform_args_constructor(
        vector3d_args_constructor(1, 2, 3), vector3d_args_constructor(4, 5, 6)
    )
    generic_clone_test(self, transf, transform_copy, clone)

    print("--Test %s END--" % test_name)


def test_clone_wave(
    self,
    wave_constructor,
    wave_copy,
    vector3d_args_constructor,
    enum_wave,
    clone,
):
    test_name = "Clone Wave binding"
    print("\n--Test %s BEGIN--" % test_name)

    w = wave_constructor()
    w.type = enum_wave().DIVERGING_WAVE
    w.time_zero = 42
    w.time_zero_reference_point = vector3d_args_constructor(7, 8, 9)
    w.parameters = [12.34, 56.7]

    generic_clone_test(self, w, wave_copy, clone)

    print("--Test %s END--" % test_name)


def test_clone_impulse_response(
    self,
    impulse_response_constructor,
    impulse_response_copy,
    clone,
):
    test_name = "Clone ImpulseResponse binding"
    print("\n--Test %s BEGIN--" % test_name)

    ir = impulse_response_constructor()
    ir.sampling_frequency = 120e6
    ir.time_offset = 100e-6
    ir.units = "azerty"
    ir.data = [12.34, 56.7, 7.89e10]

    generic_clone_test(self, ir, impulse_response_copy, clone)

    print("--Test %s END--" % test_name)


def test_clone_element_geometry(
    self,
    element_geometry_constructor,
    element_geometry_copy,
    vector3d_args_constructor,
    clone,
):
    test_name = "Clone ElementGeometry binding"
    print("\n--Test %s BEGIN--" % test_name)

    eg = element_geometry_constructor()
    eg.perimeter.append(vector3d_args_constructor(0, 0, 0))
    eg.perimeter.append(vector3d_args_constructor(0, 1, 0))
    eg.perimeter.append(vector3d_args_constructor(1, 1, 0))
    eg.perimeter.append(vector3d_args_constructor(1, 0, 0))

    generic_clone_test(self, eg, element_geometry_copy, clone)

    print("--Test %s END--" % test_name)


def test_clone_element(
    self,
    element_constructor,
    element_copy,
    element_geometry_constructor,
    impulse_response_constructor,
    transform_args_constructor,
    vector3d_args_constructor,
    clone,
):
    test_name = "Clone Element binding"
    print("\n--Test %s BEGIN--" % test_name)

    transf = transform_args_constructor(
        vector3d_args_constructor(1, 2, 3), vector3d_args_constructor(4, 5, 6)
    )

    eg = element_geometry_constructor()
    eg.perimeter.append(vector3d_args_constructor(0, 0, 0))
    eg.perimeter.append(vector3d_args_constructor(0, 1, 0))
    eg.perimeter.append(vector3d_args_constructor(1, 1, 0))
    eg.perimeter.append(vector3d_args_constructor(1, 0, 0))

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

    print("--Test %s END--" % test_name)


def test_clone_excitation(
    self,
    excitation_constructor,
    excitation_copy,
    clone,
):
    test_name = "Clone Element binding"
    print("\n--Test %s BEGIN--" % test_name)

    ex = excitation_constructor()
    ex.pulse_shape = "pulse shape"
    ex.transmit_frequency = 10e6
    ex.waveform = np.arange(100, dtype=np.float64)

    generic_clone_test(self, ex, excitation_copy, clone)

    print("--Test %s END--" % test_name)


def test_clone_transmit_setup(
    self,
    transmit_setup_constructor,
    transmit_setup_copy,
    probe_constructor,
    excitation_constructor,
    transform_args_constructor,
    vector3d_args_constructor,
    enum_wave,
    clone,
):
    test_name = "Clone TransmitSetup binding"
    print("\n--Test %s BEGIN--" % test_name)

    probe = probe_constructor()
    probe.description = "undefined probe"

    ex = excitation_constructor()
    ex.transmit_frequency = 10e6

    ex2 = excitation_constructor()
    ex2.pulse_shape = "pulse shape"

    ts = transmit_setup_constructor()
    ts.probe = probe
    ts.probe_transform = transform_args_constructor(
        vector3d_args_constructor(5.1e-120, -8, 7.0), vector3d_args_constructor(5.2, 4.3, 8.2e10)
    )
    ts.time_offset = 12052
    ts.wave.type = enum_wave().CONVERGING_WAVE
    ts.wave.time_zero_reference_point = vector3d_args_constructor(1, 2, 3)
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

    print("--Test %s END--" % test_name)


def test_clone_receive_setup(
    self,
    receive_setup_constructor,
    receive_setup_copy,
    probe_constructor,
    transform_args_constructor,
    vector3d_args_constructor,
    clone,
):
    test_name = "Clone ReceiveSetup binding"
    print("\n--Test %s BEGIN--" % test_name)

    probe = probe_constructor()
    probe.description = "undefined probe"

    rs = receive_setup_constructor()
    rs.probe = probe
    rs.probe_transform = transform_args_constructor(
        vector3d_args_constructor(5.1e-120, -8, 7.0), vector3d_args_constructor(5.2, 4.3, 8.2e10)
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

    print("--Test %s END--" % test_name)


def test_clone_event(
    self,
    event_constructor,
    event_copy,
    clone,
):
    test_name = "Clone Event binding"
    print("\n--Test %s BEGIN--" % test_name)

    event = event_constructor()
    generic_clone_test(self, event, event_copy, clone)

    print("--Test %s END--" % test_name)


def test_clone_group(
    self,
    group_constructor,
    group_copy,
    event_constructor,
    enum_sampling,
    enum_data,
    clone,
):
    test_name = "Clone Group binding"
    print("\n--Test %s BEGIN--" % test_name)

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

    print("--Test %s END--" % test_name)


def test_clone_group_data(
    self,
    group_data_constructor,
    group_data_copy,
    group_constructor,
    enum_sampling,
    enum_data,
    clone,
):
    test_name = "Clone GroupData binding"
    print("\n--Test %s BEGIN--" % test_name)

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

    print("--Test %s END--" % test_name)


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
    vector3d_args_constructor,
    enum_probe,
    enum_sampling,
    enum_data,
    enum_wave,
    clone,
):
    test_name = "Clone Probe binding"
    print("\n--Test %s BEGIN--" % test_name)

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
        vector3d_args_constructor,
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

    # PROBE 1

    probe_1 = d.acquisition.probes[1]
    probe_1_cloned = clone(probe_1)

    self.assertEqual(probe_1_cloned.element_geometries[0], element_geometry_constructor())
    self.assertEqual(
        probe_1_cloned.element_geometries[1].perimeter,
        [
            vector3d_args_constructor(15, 23, 543),
            vector3d_args_constructor(2.2, 1.4, 0.2),
        ],
    )
    self.assertEqual(
        probe_1_cloned.element_geometries[2].perimeter,
        [
            vector3d_args_constructor(2.2, 1.4, 0.2),
            vector3d_args_constructor(1.2, 2.4, 543),
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
        vector3d_args_constructor(15, 23, 543),
        vector3d_args_constructor(2.2, 1.4, 0.2),
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
            vector3d_args_constructor(123, 4.5, 5.4),
            vector3d_args_constructor(1.1, 22.1, 5.4),
        ],
    )
    self.assertEqual(
        probe_2_cloned.element_geometries[2].perimeter,
        [
            vector3d_args_constructor(5.5, 6.6, 7.7),
            vector3d_args_constructor(1.1, 0.0, 5.5),
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
        vector3d_args_constructor(15, 23, 543),
        vector3d_args_constructor(2.2, 1.4, 0.2),
    ]
    self.assertEqual(probe_2.element_geometries[0], element_geometry_constructor())
    probe_2_cloned.element_geometries.append(element_geometry_constructor())
    self.assertEqual(len(probe_2.element_geometries) + 1, len(probe_2_cloned.element_geometries))

    probe_2_cloned.impulse_responses[0].units = "toto"
    self.assertEqual(probe_2.impulse_responses[0], impulse_response_constructor())
    probe_2_cloned.impulse_responses.append(impulse_response_constructor())
    self.assertEqual(len(probe_2.impulse_responses) + 1, len(probe_2_cloned.impulse_responses))

    self.assertNotEqual(probe_2, probe_2_cloned)

    print("--Test %s END--" % test_name)


def test_clone_acquisition(
    self,
    acq_copy,
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
    vector3d_args_constructor,
    enum_probe,
    enum_sampling,
    enum_data,
    enum_wave,
    clone,
):
    test_name = "Clone Acquisition binding"
    print("\n--Test %s BEGIN--" % test_name)

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
        vector3d_args_constructor,
        enum_probe,
        enum_sampling,
        enum_data,
        enum_wave,
    )

    acq = d.acquisition
    acq_cloned = clone(acq)
    self.assertEqual(acq, acq_cloned)

    generic_clone_test(self, acq, acq_copy, clone)

    for g_id in range(len(d.acquisition.groups)):
        self.assertEqual(acq_cloned.groups[g_id], acq.groups[g_id])
        if acq.groups[g_id] is not None:
            acq_cloned.groups[g_id].description = "toto"
            self.assertNotEqual(acq_cloned.groups[g_id], acq.groups[g_id])
            acq_cloned.groups[g_id].description = acq.groups[g_id].description

    for ex_id in range(len(d.acquisition.excitations)):
        self.assertEqual(acq_cloned.excitations[ex_id], acq.excitations[ex_id])
        if acq.excitations[ex_id] is not None:
            acq_cloned.excitations[ex_id].pulse_shape = "toto"
            self.assertNotEqual(acq_cloned.excitations[ex_id], acq.excitations[ex_id])
            acq_cloned.excitations[ex_id].pulse_shape = acq.excitations[ex_id].pulse_shape

    for p_id in range(len(d.acquisition.probes)):
        self.assertEqual(acq_cloned.probes[p_id], acq.probes[p_id])
        if acq.probes[p_id] is not None:
            acq_cloned.probes[p_id].description = "toto"
            self.assertNotEqual(acq_cloned.probes[p_id], acq.probes[p_id])
            acq_cloned.probes[p_id].description = acq.probes[p_id].description

    for gd_id in range(len(d.acquisition.groups_data)):
        self.assertEqual(acq_cloned.groups_data[gd_id], acq.groups_data[gd_id])
        if acq.groups_data[gd_id] is not None:
            acq_cloned.groups_data[gd_id].group_timestamp = 42
            self.assertNotEqual(acq_cloned.groups_data[gd_id], acq.groups_data[gd_id])
            acq_cloned.groups_data[gd_id].group_timestamp = acq.groups_data[gd_id].group_timestamp

        g_id = -1
        for i in range(len(acq_cloned.groups)):
            if acq_cloned.groups[i] == acq_cloned.groups_data[gd_id].group:
                g_id = i
                break
        if g_id < 0:
            self.assertEqual(acq_cloned.groups_data[gd_id].group, None)
        else:
            self.assertEqual(
                acq_cloned.groups_data[gd_id].group,
                acq_cloned.groups[g_id],
            )
            self.assertEqual(acq_cloned.groups_data[gd_id].group, acq.groups[g_id])

    for g_id in range(len(d.acquisition.groups)):
        if acq_cloned.groups[g_id] is not None:
            group = acq_cloned.groups[g_id]
            for e_id in range(len(group.sequence)):
                receive_setup = group.sequence[e_id].receive_setup
                transmit_setup = group.sequence[e_id].transmit_setup

                p_id = -1
                for i in range(len(acq_cloned.probes)):
                    if acq_cloned.probes[i] == receive_setup.probe:
                        p_id = i
                        break
                if p_id < 0:
                    self.assertEqual(receive_setup.probe, None)
                else:
                    self.assertEqual(
                        receive_setup.probe,
                        acq_cloned.probes[p_id],
                    )

                p_id = -1
                for i in range(len(acq_cloned.probes)):
                    if acq_cloned.probes[i] == transmit_setup.probe:
                        p_id = i
                        break
                if p_id < 0:
                    self.assertEqual(transmit_setup.probe, None)
                else:
                    self.assertEqual(
                        transmit_setup.probe,
                        acq_cloned.probes[p_id],
                    )

                for ex_id in range(len(transmit_setup.excitations)):
                    acq_ex_id = -1
                    for i in range(len(acq.excitations)):
                        if acq.excitations[i] == transmit_setup.excitations[ex_id]:
                            acq_ex_id = i
                            break
                    if acq_ex_id < 0:
                        self.assertEqual(transmit_setup.excitations[ex_id], None)
                    else:
                        self.assertEqual(
                            transmit_setup.excitations[ex_id],
                            acq_cloned.excitations[acq_ex_id],
                        )

    print("--Test %s END--" % test_name)


def test_clone_dataset(
    self,
    dataset_copy,
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
    vector3d_args_constructor,
    enum_probe,
    enum_sampling,
    enum_data,
    enum_wave,
    clone,
):
    test_name = "Clone Dataset binding"
    print("\n--Test %s BEGIN--" % test_name)

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
        vector3d_args_constructor,
        enum_probe,
        enum_sampling,
        enum_data,
        enum_wave,
    )

    generic_clone_test(self, d, dataset_copy, clone)

    print("--Test %s END--" % test_name)
