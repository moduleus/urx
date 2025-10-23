import unittest

import numpy as np
import ultrasound_rawdata_exchange as urx
from parameterized import parameterized
from test_bindings_acquisition import test_acquisition
from test_bindings_clone import test_clone_dataset
from test_bindings_dataset import test_dataset
from test_bindings_double_nan import test_double_nan
from test_bindings_element import test_element
from test_bindings_element_geometry import test_element_geometry
from test_bindings_event import test_event
from test_bindings_excitation import test_excitation
from test_bindings_group import test_group
from test_bindings_group_data import (test_group_data,
                                      test_group_data_huge_transform,
                                      test_raw_data_float_complex,
                                      test_raw_data_memory_leak_fixed,
                                      test_raw_data_universal_complex,
                                      test_raw_data_universal_real)
from test_bindings_impulse_response import test_impulse_response
from test_bindings_io import test_io, test_io_stream, test_writing_opened_file
from test_bindings_probe import test_probe
from test_bindings_receive_setup import test_receive_setup
from test_bindings_std_vector import (test_vec_element,
                                      test_vec_element_geometry_ptr,
                                      test_vec_event, test_vec_excitation_ptr,
                                      test_vec_float64,
                                      test_vec_impulse_response_ptr,
                                      test_vec_uint32, test_vec_vec_float64,
                                      test_vec_vec_uint32, test_vec_vector3d)
from test_bindings_transform import test_transform
from test_bindings_transmit_setup import test_transmit_setup
from test_bindings_utils import test_utils_io
from test_bindings_vector import test_vector2d, test_vector3d
from test_bindings_version import test_version
from test_bindings_wave import test_wave


class TestBindings(unittest.TestCase):

    def test_acquisition_run(self):
        test_acquisition(
            self,
            acq_constructor=lambda: urx.Acquisition(),
            acq_copy=lambda copy: urx.Acquisition(copy),
            double_nan_constructor=lambda: urx.DoubleNan(),
            double_nan_copy=lambda copy: urx.DoubleNan(copy),
            probe_constructor=lambda: urx.Probe(),
            probe_copy=lambda copy: urx.Probe(copy),
            excitation_constructor=lambda: urx.Excitation(),
            excitation_copy=lambda copy: urx.Excitation(copy),
            group_constructor=lambda: urx.Group(),
            group_copy=lambda copy: urx.Group(copy),
            group_data_constructor=lambda: urx.GroupData(),
            group_data_copy=lambda copy: urx.GroupData(copy),
            enum_probe=lambda: urx.ProbeType,
            enum_sampling=lambda: urx.SamplingType,
        )

    def test_dataset_run(self):
        test_dataset(
            self,
            dataset_constructor=lambda: urx.Dataset(),
            dataset_copy=lambda copy: urx.Dataset(copy),
            dataset_constructor_args=lambda acquisition, version: urx.Dataset(acquisition, version),
            acq_constructor=lambda: urx.Acquisition(),
            version_copy=lambda: urx.Version(),
        )

    def test_double_nan_run(self):
        test_double_nan(
            self,
            double_nan_constructor=lambda: urx.DoubleNan(),
            double_nan_copy=lambda copy: urx.DoubleNan(copy),
        )

    def test_element_geometry_run(self):
        test_element_geometry(
            self,
            vector3d_constructor=lambda: urx.Vector3D(),
            vector3d_args=lambda x, y, z: urx.Vector3D(x, y, z),
            element_geometry_constructor=lambda: urx.ElementGeometry(),
            element_geometry_copy=lambda copy: urx.ElementGeometry(copy),
            element_geometry_args=lambda perimeter: urx.ElementGeometry(perimeter),
            vec_vector3d_args=lambda data: urx.VecVector3D(data),
        )

    def test_element_run(self):
        test_element(
            self,
            vector3d_constructor=lambda: urx.Vector3D(),
            vector3d_args=lambda x, y, z: urx.Vector3D(x, y, z),
            element_constructor=lambda: urx.Element(),
            element_copy=lambda copy: urx.Element(copy),
            element_args=lambda transform, element_geometry, impulse_response: urx.Element(
                transform, element_geometry, impulse_response
            ),
            transform_constructor=lambda: urx.Transform(),
            transform_args=lambda rotation, translation: urx.Transform(rotation, translation),
            element_geometry_constructor=lambda: urx.ElementGeometry(),
            element_geometry_args=lambda perimeter: urx.ElementGeometry(perimeter),
            impulse_response_constructor=lambda: urx.ImpulseResponse(),
            impulse_response_args=lambda sampling_frequency, time_offset, units, data: urx.ImpulseResponse(
                sampling_frequency, time_offset, units, data
            ),
        )

    def test_event_run(self):
        test_event(
            self,
            event_constructor=lambda: urx.Event(),
            event_copy=lambda copy: urx.Event(copy),
            event_args=lambda transmit_setup, receive_setup: urx.Event(
                transmit_setup, receive_setup
            ),
            transmit_setup_constructor=lambda: urx.TransmitSetup(),
            transmit_setup_args=lambda probe, wave, active_elements, excitations, delays, probe_transform, time_offset: urx.TransmitSetup(
                probe,
                wave,
                active_elements,
                excitations,
                delays,
                probe_transform,
                time_offset,
            ),
            receive_setup_constructor=lambda: urx.ReceiveSetup(),
            receive_setup_args=lambda probe, probe_transform, sampling_frequency, number_samples, active_elements, tgc_profile, tgc_sampling_frequency, modulation_frequency, time_offset: urx.ReceiveSetup(
                probe,
                probe_transform,
                sampling_frequency,
                number_samples,
                active_elements,
                tgc_profile,
                tgc_sampling_frequency,
                modulation_frequency,
                time_offset,
            ),
            probe_constructor=lambda: urx.Probe(),
            wave_constructor=lambda: urx.Wave(),
            transform_constructor=lambda: urx.Transform(),
        )

    def test_excitation_run(self):
        test_excitation(
            self,
            excitation_constructor=lambda: urx.Excitation(),
            excitation_copy=lambda copy: urx.Excitation(copy),
            excitation_args=lambda pulse_shape, transmit_frequency, sampling_frequency, waveform: urx.Excitation(
                pulse_shape, transmit_frequency, sampling_frequency, waveform
            ),
            double_nan_constructor=lambda: urx.DoubleNan(),
            double_nan_copy=lambda copy: urx.DoubleNan(copy),
            vec_float64_args=lambda data: urx.VecFloat64(data),
        )

    @parameterized.expand(
        [
            ["Do computation on data vector 100", 100],
            ["Do computation on data vector 100000", 100000],
            ["Do computation on data vector 1000000", 1000000],
            ["Do computation on data vector 10000000", 10000000],
            ["Do computation on data vector 100000000", 100000000],
        ]
    )
    def test_group_data_huge_transform_run(self, test_name, data_size):
        test_group_data_huge_transform(self, test_name, data_size, lambda: urx.GroupData())

    @parameterized.expand(
        [
            ["Complex complex64", np.complex64, np.float32],
            ["Complex complex128", np.complex128, np.float64],
        ]
    )
    def test_raw_data_float_complex_run(
        self,
        test_name,
        np_comp_type,
        np_type,
    ):
        test_raw_data_float_complex(
            self,
            test_name,
            np_comp_type,
            np_type,
            group_data_constructor=lambda: urx.GroupData(),
        )

    @parameterized.expand(
        [
            ["Complex int16", np.int16],
            ["Complex int32", np.int32],
            ["Complex float32", np.float32],
            ["Complex float64", np.float64],
        ]
    )
    def test_raw_data_universal_complex_run(
        self,
        test_name,
        np_type,
    ):
        test_raw_data_universal_complex(
            self,
            test_name,
            np_type,
            group_data_constructor=lambda: urx.GroupData(),
        )

    @parameterized.expand(
        [
            ["Real int16", np.int16],
            ["Real int32", np.int32],
            ["Real float32", np.float32],
            ["Real float64", np.float64],
        ]
    )
    def test_raw_data_universal_real_run(
        self,
        test_name,
        np_type,
    ):
        test_raw_data_universal_real(
            self,
            test_name,
            np_type,
            group_data_constructor=lambda: urx.GroupData(),
        )

    @parameterized.expand(
        [
            ["Real int16", np.int16],
            ["Real int32", np.int32],
            ["Real float32", np.float32],
            ["Real float64", np.float64],
        ]
    )
    def test_raw_data_memory_leak_fixed_run(self, test_name, np_type):
        test_raw_data_memory_leak_fixed(self, test_name, np_type)

    def test_group_data_run(
        self,
    ):
        test_group_data(
            self,
            group_data_constructor=lambda: urx.GroupData(),
            group_data_copy=lambda copy: urx.GroupData(copy),
            double_nan_constructor=lambda: urx.DoubleNan(),
            double_nan_args=lambda data: urx.DoubleNan(data),
            vec_float64_constructor=lambda: urx.VecFloat64(),
            vec_float64_args=lambda data: urx.VecFloat64(data),
            vec_vec_float64_constructor=lambda: urx.VecVecFloat64(),
            vec_vec_float64_args=lambda data: urx.VecVecFloat64(data),
            group_constructor=lambda: urx.Group(),
        )

    def test_group_run(
        self,
    ):
        test_group(
            self,
            event_constructor=lambda: urx.Event(),
            event_args=lambda transmit_setup, receive_setup: urx.Event(
                transmit_setup, receive_setup
            ),
            vec_event_constructor=lambda: urx.VecEvent(),
            enum_sampling=lambda: urx.SamplingType,
            enum_data=lambda: urx.DataType,
            group_constructor=lambda: urx.Group(),
            group_copy=lambda copy: urx.Group(copy),
            group_args=lambda sampling_type, data_type, description, sound_speed, sequence: urx.Group(
                sampling_type, data_type, description, sound_speed, sequence
            ),
            receive_setup_args=lambda probe, probe_transform, sampling_frequency, number_samples, active_elements, tgc_profile, tgc_sampling_frequency, modulation_frequency, time_offset: urx.ReceiveSetup(
                probe,
                probe_transform,
                sampling_frequency,
                number_samples,
                active_elements,
                tgc_profile,
                tgc_sampling_frequency,
                modulation_frequency,
                time_offset,
            ),
            transmit_setup_args=lambda probe, wave, active_elements, excitations, delays, probe_transform, time_offset: urx.TransmitSetup(
                probe,
                wave,
                active_elements,
                excitations,
                delays,
                probe_transform,
                time_offset,
            ),
            probe_constructor=lambda: urx.Probe(),
            wave_constructor=lambda: urx.Wave(),
            transform_constructor=lambda: urx.Transform(),
            double_nan_constructor=lambda: urx.DoubleNan(),
            double_nan_copy=lambda copy: urx.DoubleNan(copy),
        )

    def test_impulse_response_run(
        self,
    ):
        test_impulse_response(
            self,
            impulse_response_constructor=lambda: urx.ImpulseResponse(),
            impulse_response_copy=lambda copy: urx.ImpulseResponse(copy),
            impulse_response_args=lambda sampling_frequency, time_offset, units, data: urx.ImpulseResponse(
                sampling_frequency, time_offset, units, data
            ),
            double_nan_constructor=lambda: urx.DoubleNan(),
            double_nan_args=lambda arg1: urx.DoubleNan(arg1),
            vec_float64_args=lambda data: urx.VecFloat64(data),
        )

    def test_io_stream_run(
        self,
    ):
        test_io_stream(
            self,
            dataset_constructor=lambda: urx.Dataset(),
            probe_constructor=lambda: urx.Probe(),
            excitation_constructor=lambda: urx.Excitation(),
            group_constructor=lambda: urx.Group(),
            group_data_constructor=lambda: urx.GroupData(),
            element_geometry_constructor=lambda: urx.ElementGeometry(),
            element_constructor=lambda: urx.Element(),
            event_constructor=lambda: urx.Event(),
            impulse_response_constructor=lambda: urx.ImpulseResponse(),
            transform_args_constructor=lambda rotation, translation: urx.Transform(
                rotation, translation
            ),
            vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
            enum_probe=lambda: urx.ProbeType,
            enum_sampling=lambda: urx.SamplingType,
            enum_data=lambda: urx.DataType,
            enum_wave=lambda: urx.WaveType,
        )

    def test_writing_opened_file_run(
        self,
    ):
        test_writing_opened_file(
            self,
            dataset_constructor=lambda: urx.Dataset(),
            probe_constructor=lambda: urx.Probe(),
            excitation_constructor=lambda: urx.Excitation(),
            group_constructor=lambda: urx.Group(),
            group_data_constructor=lambda: urx.GroupData(),
            element_geometry_constructor=lambda: urx.ElementGeometry(),
            element_constructor=lambda: urx.Element(),
            event_constructor=lambda: urx.Event(),
            impulse_response_constructor=lambda: urx.ImpulseResponse(),
            transform_args_constructor=lambda rotation, translation: urx.Transform(
                rotation, translation
            ),
            vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
            enum_probe=lambda: urx.ProbeType,
            enum_sampling=lambda: urx.SamplingType,
            enum_data=lambda: urx.DataType,
            enum_wave=lambda: urx.WaveType,
        )

    def test_io_run(
        self,
    ):
        test_io(self)

    def test_probe_run(
        self,
    ):
        test_probe(
            self,
            vector3d_constructor=lambda: urx.Vector3D(),
            vector3d_args=lambda x, y, z: urx.Vector3D(x, y, z),
            probe_constructor=lambda: urx.Probe(),
            probe_copy=lambda copy: urx.Probe(copy),
            probe_args=lambda description, type, transform, element_geometries, impulse_responses, elements: urx.Probe(
                description,
                type,
                transform,
                element_geometries,
                impulse_responses,
                elements,
            ),
            transform_constructor=lambda: urx.Transform(),
            transform_args=lambda rotation, translation: urx.Transform(rotation, translation),
            element_geometry_args=lambda perimeter: urx.ElementGeometry(perimeter),
            impulse_response_constructor=lambda: urx.ImpulseResponse(),
            impulse_response_args=lambda sampling_frequency, time_offset, units, data: urx.ImpulseResponse(
                sampling_frequency, time_offset, units, data
            ),
            vec_element_geometry_ptr_constructor=lambda: urx.VecElementGeometryPtr(),
            vec_element_geometry_ptr_args=lambda data: urx.VecElementGeometryPtr(data),
            vec_impulse_response_ptr_constructor=lambda: urx.VecImpulseResponsePtr(),
            vec_impulse_response_ptr_args=lambda data: urx.VecImpulseResponsePtr(data),
            element_constructor=lambda: urx.Element(),
            element_args=lambda transform, element_geometry, impulse_response: urx.Element(
                transform, element_geometry, impulse_response
            ),
            vec_element_constructor=lambda: urx.VecElement(),
            vec_element_args=lambda data: urx.VecElement(data),
            enum_probe=lambda: urx.ProbeType,
        )

    def test_receive_setup_run(
        self,
    ):
        test_receive_setup(
            self,
            receive_setup_constructor=urx.ReceiveSetup,
            receive_setup_copy=urx.ReceiveSetup,
            receive_setup_args=lambda probe, probe_transform, sampling_frequency, number_samples, active_elements, tgc_profile, tgc_sampling_frequency, modulation_frequency, time_offset: urx.ReceiveSetup(
                probe,
                probe_transform,
                sampling_frequency,
                number_samples,
                active_elements,
                tgc_profile,
                tgc_sampling_frequency,
                modulation_frequency,
                time_offset,
            ),
            probe_constructor=lambda: urx.Probe(),
            probe_copy=lambda copy: urx.Probe(copy),
            transform_constructor=lambda: urx.Transform(),
            transform_args=lambda rotation, translation: urx.Transform(rotation, translation),
            double_nan_constructor=lambda: urx.DoubleNan(),
            double_nan_args=lambda data: urx.DoubleNan(data),
            vec_vec_uint32_constructor=lambda: urx.VecVecUInt32(),
            vec_float64_constructor=lambda: urx.VecFloat64(),
            vector3d_constructor=lambda: urx.Vector3D(),
            vector3d_args=lambda x, y, z: urx.Vector3D(x, y, z),
        )

    def test_vec_float64_run(
        self,
    ):
        test_vec_float64(
            self,
            vec_float64_constructor=lambda: urx.VecFloat64(),
            vec_float64_args=lambda data: urx.VecFloat64(data),
        )

    def test_vec_uint32_run(
        self,
    ):
        test_vec_uint32(
            self,
            vec_uint32_constructor=lambda: urx.VecUInt32(),
            vec_uint32_args=lambda data: urx.VecUInt32(data),
        )

    def test_vec_vec_uint32_run(
        self,
    ):
        test_vec_vec_uint32(
            self,
            vec_vec_uint32_constructor=lambda: urx.VecVecUInt32(),
            vec_vec_uint32_args=lambda data: urx.VecVecUInt32(data),
        )

    def test_vec_vec_float64_run(
        self,
    ):
        test_vec_vec_float64(
            self,
            vec_vec_float64_constructor=lambda: urx.VecVecFloat64(),
            vec_vec_float64_args=lambda data: urx.VecVecFloat64(data),
        )

    def test_vec_vector3d_run(
        self,
    ):
        test_vec_vector3d(
            self,
            vector3d_constructor=lambda: urx.Vector3D(),
            vector3d_copy=lambda copy: urx.Vector3D(copy),
            vector3d_args=lambda x, y, z: urx.Vector3D(x, y, z),
            vec_vector3d_args=lambda data: urx.VecVector3D(data),
        )

    def test_vec_element_geometry_ptr_run(
        self,
    ):
        test_vec_element_geometry_ptr(
            self,
            vector3d_constructor=lambda: urx.Vector3D(),
            vector3d_args=lambda x, y, z: urx.Vector3D(x, y, z),
            element_geometry_args=lambda perimeter: urx.ElementGeometry(perimeter),
            vec_element_geometry_ptr_args=lambda data: urx.VecElementGeometryPtr(data),
        )

    def test_vec_impulse_response_ptr_run(
        self,
    ):
        test_vec_impulse_response_ptr(
            self,
            impulse_response_constructor=lambda: urx.ImpulseResponse(),
            impulse_response_copy=lambda copy: urx.ImpulseResponse(copy),
            impulse_response_args=lambda sampling_frequency, time_offset, units, data: urx.ImpulseResponse(
                sampling_frequency, time_offset, units, data
            ),
            vec_impulse_response_ptr_args=lambda data: urx.VecImpulseResponsePtr(data),
        )

    def test_vec_element_run(
        self,
    ):
        test_vec_element(
            self,
            vector3d_constructor=lambda: urx.Vector3D(),
            vector3d_args=lambda x, y, z: urx.Vector3D(x, y, z),
            element_constructor=lambda: urx.Element(),
            element_copy=lambda copy: urx.Element(copy),
            transform_args=lambda rotation, translation: urx.Transform(rotation, translation),
            vec_element_args=lambda data: urx.VecElement(data),
        )

    def test_vec_event_run(
        self,
    ):
        test_vec_event(
            self,
            transmit_setup_args=lambda probe, wave, active_elements, excitations, delays, probe_transform, time_offset: urx.TransmitSetup(
                probe,
                wave,
                active_elements,
                excitations,
                delays,
                probe_transform,
                time_offset,
            ),
            receive_setup_args=lambda probe, probe_transform, sampling_frequency, number_samples, active_elements, tgc_profile, tgc_sampling_frequency, modulation_frequency, time_offset: urx.ReceiveSetup(
                probe,
                probe_transform,
                sampling_frequency,
                number_samples,
                active_elements,
                tgc_profile,
                tgc_sampling_frequency,
                modulation_frequency,
                time_offset,
            ),
            probe_constructor=lambda: urx.Probe(),
            wave_constructor=lambda: urx.Wave(),
            transform_constructor=lambda: urx.Transform(),
            event_constructor=lambda: urx.Event(),
            event_copy=lambda copy: urx.Event(copy),
            event_args=lambda transmit_setup, receive_setup: urx.Event(
                transmit_setup, receive_setup
            ),
            vec_event_args=lambda data: urx.VecEvent(data),
        )

    def test_vec_excitation_ptr_run(
        self,
    ):
        test_vec_excitation_ptr(
            self,
            excitation_copy=lambda copy: urx.Excitation(copy),
            excitation_args=lambda pulse_shape, transmit_frequency, sampling_frequency, waveform: urx.Excitation(
                pulse_shape, transmit_frequency, sampling_frequency, waveform
            ),
            vec_excitation_ptr_args=lambda args: urx.VecExcitationPtr(args),
            double_nan_args=lambda arg1: urx.DoubleNan(arg1),
        )

    def test_transform_run(
        self,
    ):
        test_transform(
            self,
            transform_constructor=lambda: urx.Transform(),
            transform_copy=lambda copy: urx.Transform(copy),
            transform_args=lambda rotation, translation: urx.Transform(rotation, translation),
            vector3d_constructor=lambda: urx.Vector3D(),
            vector3d_args=lambda x, y, z: urx.Vector3D(x, y, z),
        )

    def test_transmit_setup_run(
        self,
    ):
        test_transmit_setup(
            self,
            transmit_setup_constructor=lambda: urx.TransmitSetup(),
            transmit_setup_copy=lambda copy: urx.TransmitSetup(copy),
            transmit_setup_args=lambda probe, wave, active_elements, excitations, delays, probe_transform, time_offset: urx.TransmitSetup(
                probe,
                wave,
                active_elements,
                excitations,
                delays,
                probe_transform,
                time_offset,
            ),
            vec_vec_uint32_constructor=lambda: urx.VecVecUInt32(),
            vec_excitation_ptr_constructor=lambda: urx.VecExcitationPtr(),
            vec_float64_constructor=lambda: urx.VecFloat64(),
            transform_constructor=lambda: urx.Transform(),
            transform_args=lambda rotation, translation: urx.Transform(rotation, translation),
            double_nan_args=lambda arg1: urx.DoubleNan(arg1),
            vector3d_constructor=lambda: urx.Vector3D(),
            vector3d_args=lambda x, y, z: urx.Vector3D(x, y, z),
            excitation_copy=lambda copy: urx.Excitation(copy),
            excitation_args=lambda pulse_shape, transmit_frequency, sampling_frequency, waveform: urx.Excitation(
                pulse_shape, transmit_frequency, sampling_frequency, waveform
            ),
            probe_constructor=lambda: urx.Probe(),
            probe_copy=lambda copy: urx.Probe(copy),
            wave_constructor=lambda: urx.Wave(),
            wave_copy=lambda copy: urx.Wave(copy),
            enum_wave=lambda: urx.WaveType,
        )

    def test_utils_io_run(
        self,
    ):
        test_utils_io(self)

    def test_vector2d_run(
        self,
    ):
        test_vector2d(
            self,
            vector2d_constructor=lambda: urx.Vector2D(),
            vector2d_copy=lambda copy: urx.Vector2D(copy),
            vector2d_args=lambda x, y: urx.Vector2D(x, y),
        )

    def test_vector3d_run(
        self,
    ):
        test_vector3d(
            self,
            vector3d_constructor=lambda: urx.Vector3D(),
            vector3d_copy=lambda copy: urx.Vector3D(copy),
            vector3d_args=lambda x, y, z: urx.Vector3D(x, y, z),
        )

    def test_version_run(
        self,
    ):
        test_version(
            self,
            version_constructor=lambda: urx.Version(),
            version_copy=lambda copy: urx.Version(copy),
            version_args=lambda major, minor, patch: urx.Version(major, minor, patch),
            default_major=urx.URX_VERSION_MAJOR,
            default_minor=urx.URX_VERSION_MINOR,
            default_patch=urx.URX_VERSION_PATCH,
        )

    def test_wave_run(
        self,
    ):
        test_wave(
            self,
            wave_constructor=lambda: urx.Wave(),
            wave_copy=lambda copy: urx.Wave(copy),
            wave_args=lambda type, time_zero, time_zero_reference_point, parameters: urx.Wave(
                type, time_zero, time_zero_reference_point, parameters
            ),
            enum_wave=lambda: urx.WaveType,
            double_nan_constructor=lambda: urx.DoubleNan(),
            double_nan_args=lambda arg1: urx.DoubleNan(arg1),
            vector3d_constructor=lambda: urx.Vector3D(),
            vec_float64_constructor=lambda: urx.VecFloat64(),
        )

    def test_clone_run(
        self,
    ):
        # other clone functions than Dataset are not accessible yet
        # test_clone_version(
        #     self,
        #     version_args_constructor=lambda major, minor, patch: urx.Version(major, minor, patch),
        #     version_copy=lambda copy: urx.Version(copy),
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_vector2d(
        #     self,
        #     vector2d_args_constructor=lambda x, y: urx.Vector2D(x, y),
        #     vector2d_copy=lambda copy: urx.Vector2D(copy),
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_vector3d(
        #     self,
        #     vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
        #     vector3d_copy=lambda copy: urx.Vector3D(copy),
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_transform(
        #     self,
        #     transform_args_constructor=lambda rotation, translation: urx.Transform(
        #         rotation, translation
        #     ),
        #     transform_copy=lambda copy: urx.Transform(copy),
        #     vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_wave(
        #     self,
        #     wave_constructor=lambda: urx.Wave(),
        #     wave_copy=lambda copy: urx.Wave(copy),
        #     vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
        #     enum_wave=lambda: urx.WaveType,
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_impulse_response(
        #     self,
        #     impulse_response_constructor=lambda: urx.ImpulseResponse(),
        #     impulse_response_copy=lambda copy: urx.ImpulseResponse(copy),
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_element_geometry(
        #     self,
        #     element_geometry_constructor=lambda: urx.ElementGeometry(),
        #     element_geometry_copy=lambda copy: urx.ElementGeometry(copy),
        #     vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_element(
        #     self,
        #     element_constructor=lambda: urx.Element(),
        #     element_copy=lambda copy: urx.Element(copy),
        #     element_geometry_constructor=lambda: urx.ElementGeometry(),
        #     impulse_response_constructor=lambda: urx.ImpulseResponse(),
        #     transform_args_constructor=lambda rotation, translation: urx.Transform(
        #         rotation, translation
        #     ),
        #     vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_excitation(
        #     self,
        #     excitation_constructor=lambda: urx.Excitation(),
        #     excitation_copy=lambda copy: urx.Excitation(copy),
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_transmit_setup(
        #     self,
        #     transmit_setup_constructor=lambda: urx.TransmitSetup(),
        #     transmit_setup_copy=lambda copy: urx.TransmitSetup(copy),
        #     probe_constructor=lambda: urx.Probe(),
        #     excitation_constructor=lambda: urx.Excitation(),
        #     transform_args_constructor=lambda rotation, translation: urx.Transform(
        #         rotation, translation
        #     ),
        #     vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
        #     enum_wave=lambda: urx.WaveType,
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_receive_setup(
        #     self,
        #     receive_setup_constructor=lambda: urx.ReceiveSetup(),
        #     receive_setup_copy=lambda copy: urx.ReceiveSetup(copy),
        #     probe_constructor=lambda: urx.Probe(),
        #     transform_args_constructor=lambda rotation, translation: urx.Transform(
        #         rotation, translation
        #     ),
        #     vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_event(
        #     self,
        #     event_constructor=lambda: urx.Event(),
        #     event_copy=lambda copy: urx.Event(copy),
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_group(
        #     self,
        #     group_constructor=lambda: urx.Group(),
        #     group_copy=lambda copy: urx.Group(copy),
        #     event_constructor=lambda: urx.Event(),
        #     enum_sampling=lambda: urx.SamplingType,
        #     enum_data=lambda: urx.DataType,
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_group_data(
        #     self,
        #     group_data_constructor=lambda: urx.GroupData(),
        #     group_data_copy=lambda copy: urx.GroupData(copy),
        #     group_constructor=lambda: urx.Group(),
        #     enum_sampling=lambda: urx.SamplingType,
        #     enum_data=lambda: urx.DataType,
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_probe(
        #     self,
        #     probe_copy=lambda copy: urx.Probe(copy),
        #     dataset_constructor=lambda: urx.Dataset(),
        #     probe_constructor=lambda: urx.Probe(),
        #     excitation_constructor=lambda: urx.Excitation(),
        #     group_constructor=lambda: urx.Group(),
        #     group_data_constructor=lambda: urx.GroupData(),
        #     element_geometry_constructor=lambda: urx.ElementGeometry(),
        #     element_constructor=lambda: urx.Element(),
        #     event_constructor=lambda: urx.Event(),
        #     impulse_response_constructor=lambda: urx.ImpulseResponse(),
        #     transform_args_constructor=lambda rotation, translation: urx.Transform(
        #         rotation, translation
        #     ),
        #     vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
        #     enum_probe=lambda: urx.ProbeType,
        #     enum_sampling=lambda: urx.SamplingType,
        #     enum_data=lambda: urx.DataType,
        #     enum_wave=lambda: urx.WaveType,
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        # test_clone_acquisition(
        #     self,
        #     acq_copy=lambda copy: urx.Acquisition(copy),
        #     dataset_constructor=lambda: urx.Dataset(),
        #     probe_constructor=lambda: urx.Probe(),
        #     excitation_constructor=lambda: urx.Excitation(),
        #     group_constructor=lambda: urx.Group(),
        #     group_data_constructor=lambda: urx.GroupData(),
        #     element_geometry_constructor=lambda: urx.ElementGeometry(),
        #     element_constructor=lambda: urx.Element(),
        #     event_constructor=lambda: urx.Event(),
        #     impulse_response_constructor=lambda: urx.ImpulseResponse(),
        #     transform_args_constructor=lambda rotation, translation: urx.Transform(
        #         rotation, translation
        #     ),
        #     vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
        #     enum_probe=lambda: urx.ProbeType,
        #     enum_sampling=lambda: urx.SamplingType,
        #     enum_data=lambda: urx.DataType,
        #     enum_wave=lambda: urx.WaveType,
        #     clone=lambda urx_object: urx.clone(urx_object),
        # )

        test_clone_dataset(
            self,
            dataset_copy=lambda copy: urx.Dataset(copy),
            dataset_constructor=lambda: urx.Dataset(),
            probe_constructor=lambda: urx.Probe(),
            excitation_constructor=lambda: urx.Excitation(),
            group_constructor=lambda: urx.Group(),
            group_data_constructor=lambda: urx.GroupData(),
            element_geometry_constructor=lambda: urx.ElementGeometry(),
            element_constructor=lambda: urx.Element(),
            event_constructor=lambda: urx.Event(),
            impulse_response_constructor=lambda: urx.ImpulseResponse(),
            transform_args_constructor=lambda rotation, translation: urx.Transform(
                rotation, translation
            ),
            vector3d_args_constructor=lambda x, y, z: urx.Vector3D(x, y, z),
            enum_probe=lambda: urx.ProbeType,
            enum_sampling=lambda: urx.SamplingType,
            enum_data=lambda: urx.DataType,
            enum_wave=lambda: urx.WaveType,
            clone=lambda urx_object: urx.clone(urx_object),
        )


if __name__ == "__main__":
    unittest.main()
