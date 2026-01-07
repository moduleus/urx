import os
import unittest

import numpy as np
import ultrasound_rawdata_exchange as urx
from dataset_gen import gen_fake_dataset
from filelock import FileLock


def compare_complex_arrays(arr1, arr2, rtol=1e-05, atol=1e-08):
    def is_real_array(arr):
        return arr.dtype != complex and not (arr.ndim == 2 and arr.shape[1] == 2)

    if is_real_array(arr1) and is_real_array(arr2):
        return np.allclose(arr1, arr2, rtol=rtol, atol=atol)

    def to_complex(arr):
        if arr.dtype == complex:
            return arr
        elif arr.ndim == 2 and arr.shape[1] == 2:
            return arr[:, 0] + 1j * arr[:, 1]
        else:
            return arr.astype(complex)

    complex1 = to_complex(arr1)
    complex2 = to_complex(arr2)

    return np.allclose(complex1, complex2, rtol=rtol, atol=atol)


def test_io(self):
    test_name = "IO binding"
    print("\n--Test %s BEGIN--" % test_name)
    if hasattr(urx, "loadFromFile"):
        with self.assertRaises(urx.ReadFileException):
            urx.loadFromFile("missing_file.urx")
        dataset = urx.Dataset()
        with self.assertRaises(RuntimeError):
            urx.validate(dataset)
        urx_file = "test_io.urx"
        if os.path.exists(urx_file):
            os.remove(urx_file)
        writer_options = urx.WriterOptions(False, False, False)
        urx.saveToFile(urx_file, dataset, writer_options)
        reader_options = urx.ReaderOptions()
        urx.loadFromFile(urx_file)
        urx.loadFromFile(urx_file, reader_options)
        if os.path.exists(urx_file):
            os.remove(urx_file)
        with self.assertRaises(urx.WriteFileException):
            if os.name == "nt":
                self.assertRaises(RuntimeError, urx.saveToFile, "aux", dataset)
                writer_options.check_data = False
                urx.saveToFile("aux", dataset, writer_options)

            else:
                self.assertRaises(RuntimeError, urx.saveToFile, "/", dataset)
                writer_options.check_data = False
                urx.saveToFile("/", dataset, writer_options)

        dataset2 = gen_fake_dataset(
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
        writer_options2 = urx.WriterOptions(False, False, False)
        urx_file2 = "test-python.urx"
        urx.saveToFile(urx_file2, dataset2, writer_options2)
    print("--Test %s END--" % test_name)


def test_io_stream(
    self,
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
    test_name = "Stream binding"
    print("\n--Test %s BEGIN--" % test_name)

    dataset = gen_fake_dataset(
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

    filename = "python-stream.urx"

    if os.path.exists(filename):
        os.remove(filename)

    raw_data_vector_double = np.array(
        [1.2 + 9.5j, 3.4 - 6.4j, -5.6 - 99.8j, 5.6 + 99.8j], dtype=np.complex128
    )
    raw_data_vector_double2 = np.array(
        [12.0 - 11.2j, 34.0 - 7.1j, -56.0 + 8.7j, 15.3 - 1.2j], dtype=np.complex128
    )
    raw_data_vector_short = np.array([12, 456, -12, -456], dtype=np.int16)
    raw_data_vector_short2 = np.array([987, 654, 312, -312], dtype=np.int16)

    raw_data_double = np.concatenate([raw_data_vector_double, raw_data_vector_double2])
    raw_data_short = np.concatenate([raw_data_vector_short, raw_data_vector_short2])

    stream = urx.Stream(filename, dataset)
    stream.saveToFile()
    stream.writerOptions().clean_unusable_data = True
    self.assertTrue(stream.writerOptions().clean_unusable_data)
    stream.writerOptions().clean_unusable_data = False
    self.assertFalse(stream.writerOptions().clean_unusable_data)

    group_data = stream.createGroupData(dataset.acquisition.groups[1], 1.0)
    group_data.append(raw_data_vector_double, 1.2, [2.3, 3.4, 4.5])
    group_data.append(raw_data_vector_double2, 12.0, [23.0, 34.0, 45.0])

    group_data2 = stream.createGroupData(dataset.acquisition.groups[2], 1.0)
    group_data2.append(raw_data_vector_short, 9, [8.0])
    group_data2.append(raw_data_vector_short2, 5.0, [8, 9.4])

    dataset_loaded = urx.loadFromFile(filename)

    group_data_double = dataset_loaded.acquisition.groups_data[-2]
    self.assertEqual(group_data_double.sequence_timestamps, [1.2, 12])
    self.assertEqual(group_data_double.event_timestamps, [[2.3, 3.4, 4.5], [23.0, 34.0, 45.0]])
    self.assertTrue(compare_complex_arrays(group_data_double.raw_data, raw_data_double))

    group_data_short = dataset_loaded.acquisition.groups_data[-1]
    self.assertEqual(group_data_short.sequence_timestamps, [9.0, 5.0])
    self.assertEqual(group_data_short.event_timestamps, [[8.0], [8.0, 9.4]])
    self.assertTrue(compare_complex_arrays(group_data_short.raw_data, raw_data_short))

    dataset_loaded2 = urx.Dataset()
    stream = urx.Stream(filename, dataset_loaded2)

    stream.readerOptions().raw_data_load_policy = urx.RawDataLoadPolicy.FULL
    self.assertEqual(stream.readerOptions().raw_data_load_policy, urx.RawDataLoadPolicy.FULL)
    stream.readerOptions().raw_data_load_policy = urx.RawDataLoadPolicy.STREAM
    self.assertEqual(stream.readerOptions().raw_data_load_policy, urx.RawDataLoadPolicy.STREAM)

    stream.loadFromFile()

    buffer_double = np.ndarray((8), dtype=np.complex128)
    stream.readRawData(len(dataset_loaded2.acquisition.groups_data) - 2, buffer_double, 0, 0, 1)
    stream.readRawData(len(dataset_loaded2.acquisition.groups_data) - 2, buffer_double, 1, 1, 1)
    self.assertTrue(compare_complex_arrays(buffer_double, raw_data_double))

    buffer_short = np.ndarray((8), dtype=np.int16)
    stream.readRawData(len(dataset_loaded2.acquisition.groups_data) - 1, buffer_short, 0, 0, 1)
    stream.readRawData(len(dataset_loaded2.acquisition.groups_data) - 1, buffer_short, 1, 1, 1)
    self.assertTrue(compare_complex_arrays(buffer_short, raw_data_short))

    print("--Test %s END--" % test_name)


def test_writing_opened_file(
    self,
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
    # Test is successful but it makes HDF5 / Python crashed at .
    # bindings_cp312_win_amd64!H5F_close+0xd
    # bindings_cp312_win_amd64!H5I__clear_type_cb+0x46
    # bindings_cp312_win_amd64!H5SL_try_free_safe+0x55
    # bindings_cp312_win_amd64!H5I_clear_type+0x70
    # bindings_cp312_win_amd64!H5F_term_interface+0x2a
    # bindings_cp312_win_amd64!H5_term_library+0x4ca
    # bindings_cp312_win_amd64!H5close+0x9
    # bindings_cp312_win_amd64!H5::H5Library::termH5cpp+0x9
    # bindings_cp312_win_amd64!<lambda_f03950bc5685219e0bcd2087efbe011e>::operator()+0x8a
    # bindings_cp312_win_amd64!__crt_seh_guarded_call<int>::operator()<<lambda_7777bce6b2f8c936911f934f8298dc43>,<lambda_f03950bc5685219e0bcd2087efbe011e> &,<lambda_3883c3dff614d5e0c5f61bb1ac94921c> >+0x25
    # bindings_cp312_win_amd64!__acrt_lock_and_call+0x22
    # bindings_cp312_win_amd64!_execute_onexit_table+0x3d
    # bindings_cp312_win_amd64!<lambda_6e4b09c48022b2350581041d5f6b0c4c>::operator()+0x6e
    # bindings_cp312_win_amd64!__crt_seh_guarded_call<void>::operator()<<lambda_d80eeec6fff315bfe5c115232f3240e3>,<lambda_6e4b09c48022b2350581041d5f6b0c4c> &,<lambda_2358e3775559c9db80273638284d5e45> >+0x25
    # bindings_cp312_win_amd64!__acrt_lock_and_call+0x20
    # bindings_cp312_win_amd64!common_exit+0xab
    # bindings_cp312_win_amd64!__scrt_dllmain_uninitialize_c+0x2b
    # bindings_cp312_win_amd64!dllmain_crt_process_detach+0x45
    # bindings_cp312_win_amd64!dllmain_dispatch+0xe1
    # ntdll!LdrpCallInitRoutine+0x6b
    # ntdll!LdrShutdownProcess+0x176
    # ntdll!RtlExitUserProcess+0xad
    # kernel32!ExitProcessImplementation+0xb
    # ucrtbase!exit_or_terminate_process+0x50
    # ucrtbase!common_exit+0x79
    # python+0x1297
    # kernel32!BaseThreadInitThunk+0x1d
    # ntdll!RtlUserThreadStart+0x28
    return

    test_name = "Exception raised writting opened file"
    print("\n--Test %s BEGIN--" % test_name)

    dataset = gen_fake_dataset(
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

    filename = "python-opened-file.urx"

    options = urx.WriterOptions(False, False, False)
    urx.saveToFile(filename, dataset, options)
    with FileLock(filename):
        self.assertRaises(urx.WriteFileException, urx.saveToFile, filename, dataset, options)

    print("--Test %s END--" % test_name)


if __name__ == "__main__":
    unittest.main()
