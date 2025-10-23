import numpy as np
import ultrasound_rawdata_exchange as urx


def gen_fake_dataset(
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
):

    dataset = dataset_constructor()

    dataset.version.major = urx.URX_VERSION_MAJOR
    dataset.version.minor = urx.URX_VERSION_MINOR
    dataset.version.patch = urx.URX_VERSION_PATCH

    dataset.acquisition.authors = "AuthOr"
    dataset.acquisition.description = "康Description🐜"
    dataset.acquisition.local_time = "2019-06-15T15:53:00"
    dataset.acquisition.country_code = "FR"
    dataset.acquisition.system = "???"
    dataset.acquisition.timestamp = np.nan

    dataset.acquisition.probes = [probe_constructor()]

    probe = probe_constructor()
    probe.description = "Probe 1"
    probe.type = enum_probe().MATRIX
    probe.transform = transform_args_constructor(
        vector3d_args_constructor(-1, 2.3, 4e5), vector3d_args_constructor(1.2, -2.2, 232)
    )

    probe.element_geometries.append(element_geometry_constructor())

    element_geometry = element_geometry_constructor()
    element_geometry.perimeter = [
        vector3d_args_constructor(15, 23, 543),
        vector3d_args_constructor(2.2, 1.4, 0.2),
    ]
    probe.element_geometries.append(element_geometry)

    element_geometry = element_geometry_constructor()
    element_geometry.perimeter = [
        vector3d_args_constructor(32.2, 31.4, 30.2),
        vector3d_args_constructor(1.2, 2.4, 543),
    ]
    probe.element_geometries.append(element_geometry)

    probe.impulse_responses.append(impulse_response_constructor())

    impulse_response = impulse_response_constructor()
    impulse_response.sampling_frequency = 20000001
    impulse_response.data = [1.2, 1.3, 1.4]
    impulse_response.units = "meter"
    impulse_response.time_offset = 10000
    probe.impulse_responses.append(impulse_response)

    impulse_response = impulse_response_constructor()
    impulse_response.sampling_frequency = np.nan
    impulse_response.data = [2.2, 2.3, 2.4]
    impulse_response.units = "milli"
    impulse_response.time_offset = np.nan
    probe.impulse_responses.append(impulse_response)

    probe.elements.append(element_constructor())

    element = element_constructor()
    element.transform = transform_args_constructor(
        vector3d_args_constructor(1, 2, 3), vector3d_args_constructor(4e-3, 5e-3, 6e-3)
    )
    element.element_geometry = probe.element_geometries[0]
    element.impulse_response = probe.impulse_responses[1]
    probe.elements.append(element)

    element = element_constructor()
    element.transform = transform_args_constructor(
        vector3d_args_constructor(-1, -2, -3), vector3d_args_constructor(4e-3, 5e-3, 6e-3)
    )
    element.element_geometry = probe.element_geometries[1]
    element.impulse_response = probe.impulse_responses[2]
    probe.elements.append(element)

    element = element_constructor()
    element.transform = transform_args_constructor(
        vector3d_args_constructor(1.23, 4.56, 7.89), vector3d_args_constructor(10, -11.1, 12.3)
    )
    element.element_geometry = probe.element_geometries[1]
    element.impulse_response = probe.impulse_responses[0]
    probe.elements.append(element)

    element = element_constructor()
    element.transform = transform_args_constructor(
        vector3d_args_constructor(-33, 2.2, 44), vector3d_args_constructor(77, 8.8, -97.8e5)
    )
    element.element_geometry = probe.element_geometries[2]
    element.impulse_response = probe.impulse_responses[1]
    probe.elements.append(element)

    dataset.acquisition.probes.append(probe)

    probe = probe_constructor()
    probe.description = "Probe 2"
    probe.type = enum_probe()(123)
    probe.transform = transform_args_constructor(
        vector3d_args_constructor(3, 4, 5), vector3d_args_constructor(3.2, 4.2, 5.2)
    )

    probe.element_geometries.append(element_geometry_constructor())

    element_geometry = element_geometry_constructor()
    element_geometry.perimeter = [
        vector3d_args_constructor(123, 4.5, 5.4),
        vector3d_args_constructor(1.1, 22.1, 5.4),
    ]
    probe.element_geometries.append(element_geometry)

    element_geometry = element_geometry_constructor()
    element_geometry.perimeter = [
        vector3d_args_constructor(5.5, 6.6, 7.7),
        vector3d_args_constructor(1.1, 0.0, 5.5),
    ]
    probe.element_geometries.append(element_geometry)

    probe.impulse_responses.append(impulse_response_constructor())

    impulse_response = impulse_response_constructor()
    impulse_response.sampling_frequency = 20000011
    impulse_response.data = [22.2, 3.3, 144.4]
    impulse_response.units = "met55er"
    impulse_response.time_offset = 100500
    probe.impulse_responses.append(impulse_response)

    impulse_response = impulse_response_constructor()
    impulse_response.sampling_frequency = 5000004402.0
    impulse_response.data = [23.2, 2.53, 72.4]
    impulse_response.units = "mi3lli"
    impulse_response.time_offset = 200100
    probe.impulse_responses.append(impulse_response)

    element = element_constructor()
    element.transform = transform_args_constructor(
        vector3d_args_constructor(-99, 8.8, 77), vector3d_args_constructor(74.1, 8.52, -0.963)
    )
    element.element_geometry = probe.element_geometries[1]
    element.impulse_response = probe.impulse_responses[1]
    probe.elements.append(element)

    element = element_constructor()
    element.transform = transform_args_constructor(
        vector3d_args_constructor(3.3, 11, 2.2), vector3d_args_constructor(-86, -75.3, 42.0)
    )
    element.element_geometry = probe.element_geometries[2]
    element.impulse_response = probe.impulse_responses[2]
    probe.elements.append(element)
    dataset.acquisition.probes.append(probe)

    dataset.acquisition.excitations.append(excitation_constructor())

    excitation = excitation_constructor()
    excitation.pulse_shape = "sinusoidal"
    excitation.transmit_frequency = 123456789
    excitation.sampling_frequency = 987654321
    excitation.waveform = [0, 0.1, 0.2]
    dataset.acquisition.excitations.append(excitation)

    excitation = excitation_constructor()
    excitation.pulse_shape = "square wave"
    excitation.transmit_frequency = 741085296
    excitation.sampling_frequency = 369258147
    excitation.waveform = [1, 1.1, 1.2]
    dataset.acquisition.excitations.append(excitation)

    dataset.acquisition.groups.append(group_constructor())

    group = group_constructor()
    group.sampling_type = enum_sampling().IQ
    group.data_type = enum_data().DOUBLE
    group.sound_speed = 1500.0
    group.sequence.append(event_constructor())

    event = event_constructor()
    event.transmit_setup.probe = dataset.acquisition.probes[0]
    event.transmit_setup.probe_transform = transform_args_constructor(
        vector3d_args_constructor(5.1e-120, -8, 7.0), vector3d_args_constructor(5.2, 4.3, 8.2e10)
    )
    event.transmit_setup.time_offset = 120.2
    event.transmit_setup.wave.type = enum_wave().CONVERGING_WAVE
    event.transmit_setup.wave.time_zero_reference_point = [5, 4, 3]
    event.transmit_setup.wave.time_zero = 5.11
    event.transmit_setup.wave.parameters = [7, 53e-12, 0.2, 1, 0.3, 5.6e20, 7]
    event.transmit_setup.active_elements = [[], [0, 1], [0]]
    event.transmit_setup.excitations = [
        dataset.acquisition.excitations[1],
        dataset.acquisition.excitations[0],
    ]
    event.transmit_setup.delays = [1.2, 0.3]
    event.receive_setup.probe = dataset.acquisition.probes[1]
    event.receive_setup.probe_transform = transform_args_constructor(
        vector3d_args_constructor(5.1e-120, -8, 7.0), vector3d_args_constructor(5.2, 4.3, 8.2e10)
    )
    event.receive_setup.sampling_frequency = 125468
    event.receive_setup.number_samples = 1
    event.receive_setup.active_elements = [[1, 2, 753, 8, 6, 2, 1], []]
    event.receive_setup.tgc_profile = [1.2, 45.0e120, 4.5, -4.2e-50, 1, 0.5, 4.6, 1, 2]
    event.receive_setup.tgc_sampling_frequency = 12.5e10
    event.receive_setup.modulation_frequency = -125
    event.receive_setup.time_offset = 1205
    group.sequence.append(event)

    event = event_constructor()
    event.transmit_setup.probe = dataset.acquisition.probes[1]
    event.transmit_setup.probe_transform = transform_args_constructor(
        vector3d_args_constructor(5.1e-120, -8, 7.0), vector3d_args_constructor(5.2, 4.3, 8.2e10)
    )
    event.transmit_setup.time_offset = 12052
    event.transmit_setup.wave.type = enum_wave().CONVERGING_WAVE
    event.transmit_setup.wave.time_zero_reference_point = vector3d_args_constructor(1, 2, 3)
    event.transmit_setup.wave.time_zero = 4.11
    event.transmit_setup.wave.parameters = [7, 0.3, 5.6, 7]
    event.transmit_setup.active_elements = [[1], [0, 1]]
    event.transmit_setup.excitations = [
        dataset.acquisition.excitations[1],
        dataset.acquisition.excitations[1],
    ]
    event.transmit_setup.delays = [0.3, 1.2]
    event.receive_setup.probe = dataset.acquisition.probes[2]
    event.receive_setup.probe_transform = transform_args_constructor(
        vector3d_args_constructor(5.1e-120, -8, 7.0), vector3d_args_constructor(5.2, 4.3, 8.2e10)
    )
    event.receive_setup.sampling_frequency = 1268
    event.receive_setup.number_samples = 2
    event.receive_setup.active_elements = [[1, 2, 3]]
    event.receive_setup.tgc_profile = [1, 0.2, 4.0, 5, 0.1, 5, 0.45, 5]
    event.receive_setup.tgc_sampling_frequency = 4.5
    event.receive_setup.modulation_frequency = 4.58
    event.receive_setup.time_offset = 7.8
    group.sequence.append(event)

    group.description = "group description"
    dataset.acquisition.groups.append(group)

    group = group_constructor()
    group.sampling_type = enum_sampling().RF
    group.data_type = enum_data().INT16
    group.sound_speed = 1550.0

    group.sequence.append(event_constructor())

    event = event_constructor()
    event.transmit_setup.probe = dataset.acquisition.probes[1]
    event.transmit_setup.probe_transform = transform_args_constructor(
        vector3d_args_constructor(5.1e-120, -8, 7.0), vector3d_args_constructor(5.2, 4.3, 8.2e10)
    )
    event.transmit_setup.time_offset = 1202.2
    event.transmit_setup.wave.type = enum_wave().CYLINDRICAL_WAVE
    event.transmit_setup.wave.time_zero_reference_point.x = 3
    event.transmit_setup.wave.time_zero_reference_point.y = 4
    event.transmit_setup.wave.time_zero_reference_point.z = 5
    event.transmit_setup.wave.time_zero = 5.161
    event.transmit_setup.wave.parameters = [1, 0.2, 4, 3, 6, 4.5]
    event.transmit_setup.active_elements = [[0], [1]]
    event.transmit_setup.excitations = [
        dataset.acquisition.excitations[0],
        dataset.acquisition.excitations[1],
    ]
    event.transmit_setup.delays = [1.2, 45]
    event.receive_setup.probe = dataset.acquisition.probes[0]
    event.receive_setup.probe_transform = transform_args_constructor(
        vector3d_args_constructor(5.1e-120, -8, 7.0), vector3d_args_constructor(5.2, 4.3, 8.2e10)
    )
    event.receive_setup.sampling_frequency = 1275468
    event.receive_setup.number_samples = 2
    event.receive_setup.active_elements = [[1, 2, 5753, 58, 60, 2, 1]]
    event.receive_setup.tgc_profile = [1.2, 45.0, 4.25, -4.52, 1, 0.5, 4.36, 1, 2]
    event.receive_setup.tgc_sampling_frequency = -12.5
    event.receive_setup.modulation_frequency = 1925
    event.receive_setup.time_offset = 206
    group.sequence.append(event)

    event = event_constructor()
    event.transmit_setup.probe = dataset.acquisition.probes[0]
    event.transmit_setup.probe_transform = transform_args_constructor(
        vector3d_args_constructor(5.1e-120, -8, 7.0), vector3d_args_constructor(5.2, 4.3, 8.2e10)
    )
    event.transmit_setup.time_offset = 1202
    event.transmit_setup.wave.type = enum_wave().CONVERGING_WAVE
    event.transmit_setup.wave.time_zero_reference_point = vector3d_args_constructor(6, 5, 2)
    event.transmit_setup.wave.time_zero = 4.11
    event.transmit_setup.wave.parameters = [7, 53, 0.3, 5.6, 7]
    event.transmit_setup.active_elements = [[1]]
    event.transmit_setup.excitations = [dataset.acquisition.excitations[0]]
    event.transmit_setup.delays = [1.6]
    event.receive_setup.probe = dataset.acquisition.probes[2]
    event.receive_setup.probe_transform = transform_args_constructor(
        vector3d_args_constructor(5.1e-120, -8, 7.0), vector3d_args_constructor(5.2, 4.3, 8.2e10)
    )
    event.receive_setup.sampling_frequency = 12468
    event.receive_setup.number_samples = 2
    event.receive_setup.active_elements = [[15, 32, 3]]
    event.receive_setup.tgc_profile = [17, 0.2, 3.0, 5, 0.1, 35, 0.445, 5]
    event.receive_setup.tgc_sampling_frequency = 47.5
    event.receive_setup.modulation_frequency = 4.558
    event.receive_setup.time_offset = 7.38
    group.sequence.append(event)

    group.description = "group2 description"
    dataset.acquisition.groups.append(group)
    dataset.acquisition.groups.append(group_constructor())
    dataset.acquisition.groups[-1].sequence.append(event_constructor())

    dataset.acquisition.timestamp = 1242

    if isinstance(dataset_constructor(), urx.Dataset):
        dataset.acquisition.groups_data.append(group_data_constructor())

        group_data = group_data_constructor()
        group_data.group = dataset.acquisition.groups[2]

        group_data.raw_data = np.zeros(6, dtype=np.float64)
        group_data.raw_data[0] = 1.2
        group_data.raw_data[1] = 2.3
        group_data.raw_data[2] = 3.4
        group_data.raw_data[3] = 4.5
        group_data.raw_data[4] = 5.6
        group_data.raw_data[5] = 6.7

        group_data.group_timestamp = 283954.334
        group_data.sequence_timestamps = [1, 2, 4.2, 1, 0.5, 5.6]
        group_data.event_timestamps = [
            [1, 0.24, 1.0, 5.2, 4.5, 7, 0.964, 0.5],
            [1, 2, 4, 85, 0.15, -4.5, -7, 0.45, 0.6, 4],
        ]
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.group = dataset.acquisition.groups[1]
        group_data.raw_data = np.ndarray((4, 2), dtype=np.int16)
        group_data.raw_data[0] = [123, 456]
        group_data.raw_data[1] = [159, 753]
        group_data.raw_data[2] = [789, 456]
        group_data.raw_data[3] = [123, 753]
        group_data.group_timestamp = 123
        group_data.sequence_timestamps = [1, 2, 34]
        group_data.event_timestamps = [[4, 5, 7], [8, 7, 6]]
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.group = dataset.acquisition.groups[1]
        group_data.raw_data = np.ndarray((5, 2), dtype=np.int16)
        group_data.raw_data[0] = [123, 456]
        group_data.raw_data[1] = [159, 753]
        group_data.raw_data[2] = [789, 456]
        group_data.raw_data[3] = [123, 753]
        group_data.raw_data[4] = [-255, 15909]
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.group = dataset.acquisition.groups[0]
        group_data.raw_data = np.zeros(0, dtype=np.int32)
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.raw_data = np.array([123], dtype=np.int16)
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.raw_data = np.array([2345678], dtype=np.int32)
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.raw_data = np.array([2345.678], dtype=np.float32)
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.raw_data = np.array([234567.8], dtype=np.float64)
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.raw_data = np.array([[123, -123]], dtype=np.int16)
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.raw_data = np.array([[2345678, -2345678]], dtype=np.int32)
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.raw_data = np.array([2.0 + 345.678j], dtype=np.complex64)
        dataset.acquisition.groups_data.append(group_data)

        group_data = group_data_constructor()
        group_data.raw_data = np.array([234.0 + 567.8j], dtype=np.complex128)
        dataset.acquisition.groups_data.append(group_data)

    return dataset
