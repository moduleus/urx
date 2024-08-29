"""
Python binding of urx library
"""
from __future__ import annotations
import numpy
import typing
import typing_extensions
__all__ = ['Acquisition', 'DataType', 'Dataset', 'DoubleNan', 'Element', 'ElementGeometry', 'Event', 'Excitation', 'Group', 'GroupData', 'GroupDataReader', 'ImpulseResponse', 'Probe', 'ProbeType', 'ReceiveSetup', 'SamplingType', 'Transform', 'TransmitSetup', 'URX_VERSION_MAJOR', 'URX_VERSION_MINOR', 'URX_VERSION_PATCH', 'UrxAcquisition', 'UrxDataType', 'UrxDataset', 'UrxDoubleNan', 'UrxElement', 'UrxElementGeometry', 'UrxEvent', 'UrxExcitation', 'UrxGroup', 'UrxGroupData', 'UrxImpulseResponse', 'UrxProbe', 'UrxProbeType', 'UrxReceiveSetup', 'UrxSamplingType', 'UrxTransform', 'UrxTransmitSetup', 'UrxVector2D', 'UrxVector3D', 'UrxVersion', 'UrxWave', 'UrxWaveType', 'VecElement', 'VecElementGeometryPtr', 'VecEvent', 'VecExcitationPtr', 'VecFloat32', 'VecFloat64', 'VecGroupData', 'VecGroupPtr', 'VecImpulseResponsePtr', 'VecInt16', 'VecInt32', 'VecInt64', 'VecInt8', 'VecProbePtr', 'VecString', 'VecUInt16', 'VecUInt32', 'VecUInt64', 'VecUInt8', 'VecVecFloat32', 'VecVecFloat64', 'VecVecInt16', 'VecVecInt32', 'VecVecInt64', 'VecVecInt8', 'VecVecString', 'VecVecUInt16', 'VecVecUInt32', 'VecVecUInt64', 'VecVecUInt8', 'VecVector2D', 'VecVector3D', 'Vector2D', 'Vector3D', 'Version', 'Wave', 'WaveType', 'isIso3166', 'isIso8601', 'loadFromFile', 'pyGetFormat', 'saveToFile', 'sizeofDataType', 'sizeofSample']
class GroupDataReader:
    def __init__(self, arg0: UrxGroupData, arg1: int) -> None:
        ...
    def channelsCount(self, arg0: int) -> int:
        ...
    def eventsCount(self) -> int:
        ...
    def offset(self, arg0: int, arg1: int, arg2: int, arg3: int) -> int:
        ...
    def sampleByteSize(self) -> int:
        ...
    def samplesCount(self, arg0: int) -> int:
        ...
    def sequencesCount(self) -> int:
        ...
class UrxAcquisition:
    __hash__: typing.ClassVar[None] = None
    authors: str
    country_code: str
    description: str
    excitations: VecExcitationPtr
    groups: VecGroupPtr
    groups_data: VecGroupData
    local_time: str
    probes: VecProbePtr
    system: str
    timestamp: UrxDoubleNan
    def __eq__(self, arg0: UrxAcquisition) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxAcquisition) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: str, arg1: str, arg2: str, arg3: str, arg4: str, arg5: UrxDoubleNan, arg6: VecProbePtr, arg7: VecExcitationPtr, arg8: VecGroupPtr, arg9: VecGroupData) -> None:
        ...
    def __ne__(self, arg0: UrxAcquisition) -> bool:
        ...
class UrxDataType:
    """
    Members:
    
      INT16
    
      INT32
    
      FLOAT
    
      DOUBLE
    
      UNDEFINED
    """
    DOUBLE: typing.ClassVar[UrxDataType]  # value = <UrxDataType.DOUBLE: 3>
    FLOAT: typing.ClassVar[UrxDataType]  # value = <UrxDataType.FLOAT: 2>
    INT16: typing.ClassVar[UrxDataType]  # value = <UrxDataType.INT16: 0>
    INT32: typing.ClassVar[UrxDataType]  # value = <UrxDataType.INT32: 1>
    UNDEFINED: typing.ClassVar[UrxDataType]  # value = <UrxDataType.UNDEFINED: -1>
    __members__: typing.ClassVar[dict[str, UrxDataType]]  # value = {'INT16': <UrxDataType.INT16: 0>, 'INT32': <UrxDataType.INT32: 1>, 'FLOAT': <UrxDataType.FLOAT: 2>, 'DOUBLE': <UrxDataType.DOUBLE: 3>, 'UNDEFINED': <UrxDataType.UNDEFINED: -1>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class UrxDataset:
    __hash__: typing.ClassVar[None] = None
    acquisition: UrxAcquisition
    version: UrxVersion
    def __eq__(self, arg0: UrxDataset) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxDataset) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxAcquisition, arg1: UrxVersion) -> None:
        ...
    def __ne__(self, arg0: UrxDataset) -> bool:
        ...
class UrxDoubleNan:
    __hash__: typing.ClassVar[None] = None
    value: float
    @typing.overload
    def __add__(self, arg0: UrxDoubleNan) -> UrxDoubleNan:
        ...
    @typing.overload
    def __add__(self, arg0: float) -> UrxDoubleNan:
        ...
    @typing.overload
    def __eq__(self, arg0: UrxDoubleNan) -> bool:
        ...
    @typing.overload
    def __eq__(self, arg0: float) -> bool:
        ...
    @typing.overload
    def __iadd__(self, arg0: UrxDoubleNan) -> UrxDoubleNan:
        ...
    @typing.overload
    def __iadd__(self, arg0: float) -> UrxDoubleNan:
        ...
    @typing.overload
    def __imul__(self, arg0: UrxDoubleNan) -> UrxDoubleNan:
        ...
    @typing.overload
    def __imul__(self, arg0: float) -> UrxDoubleNan:
        ...
    @typing.overload
    def __init__(self, arg0: float) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxDoubleNan) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __isub__(self, arg0: UrxDoubleNan) -> UrxDoubleNan:
        ...
    @typing.overload
    def __isub__(self, arg0: float) -> UrxDoubleNan:
        ...
    @typing.overload
    def __itruediv__(self, arg0: UrxDoubleNan) -> UrxDoubleNan:
        ...
    @typing.overload
    def __itruediv__(self, arg0: float) -> UrxDoubleNan:
        ...
    @typing.overload
    def __mul__(self, arg0: UrxDoubleNan) -> UrxDoubleNan:
        ...
    @typing.overload
    def __mul__(self, arg0: float) -> UrxDoubleNan:
        ...
    @typing.overload
    def __ne__(self, arg0: UrxDoubleNan) -> bool:
        ...
    @typing.overload
    def __ne__(self, arg0: float) -> bool:
        ...
    def __neg__(self) -> float:
        ...
    def __pos__(self) -> float:
        ...
    def __radd__(self, arg0: float) -> UrxDoubleNan:
        ...
    def __rmul__(self, arg0: float) -> UrxDoubleNan:
        ...
    def __rsub__(self, arg0: float) -> UrxDoubleNan:
        ...
    def __rtruediv__(self, arg0: float) -> UrxDoubleNan:
        ...
    @typing.overload
    def __sub__(self, arg0: UrxDoubleNan) -> UrxDoubleNan:
        ...
    @typing.overload
    def __sub__(self, arg0: float) -> UrxDoubleNan:
        ...
    @typing.overload
    def __truediv__(self, arg0: UrxDoubleNan) -> UrxDoubleNan:
        ...
    @typing.overload
    def __truediv__(self, arg0: float) -> UrxDoubleNan:
        ...
class UrxElement:
    __hash__: typing.ClassVar[None] = None
    element_geometry: UrxElementGeometry
    impulse_response: UrxImpulseResponse
    transform: UrxTransform
    def __eq__(self, arg0: UrxElement) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxElement) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxTransform, arg1: UrxElementGeometry, arg2: UrxImpulseResponse) -> None:
        ...
    def __ne__(self, arg0: UrxElement) -> bool:
        ...
class UrxElementGeometry:
    __hash__: typing.ClassVar[None] = None
    perimeter: VecVector3D
    def __eq__(self, arg0: UrxElementGeometry) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxElementGeometry) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVector3D) -> None:
        ...
    def __ne__(self, arg0: UrxElementGeometry) -> bool:
        ...
class UrxEvent:
    __hash__: typing.ClassVar[None] = None
    receive_setup: UrxReceiveSetup
    transmit_setup: UrxTransmitSetup
    def __eq__(self, arg0: UrxEvent) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxEvent) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxTransmitSetup, arg1: UrxReceiveSetup) -> None:
        ...
    def __ne__(self, arg0: UrxEvent) -> bool:
        ...
class UrxExcitation:
    __hash__: typing.ClassVar[None] = None
    pulse_shape: str
    sampling_frequency: UrxDoubleNan
    transmit_frequency: UrxDoubleNan
    waveform: VecFloat64
    def __eq__(self, arg0: UrxExcitation) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxExcitation) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: str, arg1: UrxDoubleNan, arg2: UrxDoubleNan, arg3: VecFloat64) -> None:
        ...
    def __ne__(self, arg0: UrxExcitation) -> bool:
        ...
class UrxGroup:
    __hash__: typing.ClassVar[None] = None
    data_type: UrxDataType
    description: str
    sampling_type: UrxSamplingType
    sequence: VecEvent
    sound_speed: UrxDoubleNan
    def __eq__(self, arg0: UrxGroup) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxGroup) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxSamplingType, arg1: UrxDataType, arg2: str, arg3: UrxDoubleNan, arg4: VecEvent) -> None:
        ...
    def __ne__(self, arg0: UrxGroup) -> bool:
        ...
class UrxGroupData:
    __hash__: typing.ClassVar[None] = None
    event_timestamps: VecVecFloat64
    group: UrxGroup
    group_timestamp: UrxDoubleNan
    raw_data: numpy.ndarray
    sequence_timestamps: VecFloat64
    def __eq__(self, arg0: UrxGroupData) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxGroupData) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxGroup, arg1: numpy.ndarray, arg2: UrxDoubleNan, arg3: VecFloat64, arg4: VecVecFloat64) -> None:
        ...
    def __ne__(self, arg0: UrxGroupData) -> bool:
        ...
class UrxImpulseResponse:
    __hash__: typing.ClassVar[None] = None
    data: VecFloat64
    sampling_frequency: UrxDoubleNan
    time_offset: UrxDoubleNan
    units: str
    def __eq__(self, arg0: UrxImpulseResponse) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxImpulseResponse) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxDoubleNan, arg1: UrxDoubleNan, arg2: str, arg3: VecFloat64) -> None:
        ...
    def __ne__(self, arg0: UrxImpulseResponse) -> bool:
        ...
class UrxProbe:
    __hash__: typing.ClassVar[None] = None
    description: str
    element_geometries: VecElementGeometryPtr
    elements: VecElement
    impulse_responses: VecImpulseResponsePtr
    transform: UrxTransform
    type: UrxProbeType
    def __eq__(self, arg0: UrxProbe) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxProbe) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: str, arg1: UrxProbeType, arg2: UrxTransform, arg3: VecElementGeometryPtr, arg4: VecImpulseResponsePtr, arg5: VecElement) -> None:
        ...
    def __ne__(self, arg0: UrxProbe) -> bool:
        ...
class UrxProbeType:
    """
    Members:
    
      LINEAR
    
      CURVILINEAR
    
      RCA
    
      MATRIX
    
      SPARSE
    
      UNDEFINED
    """
    CURVILINEAR: typing.ClassVar[UrxProbeType]  # value = <UrxProbeType.CURVILINEAR: 1>
    LINEAR: typing.ClassVar[UrxProbeType]  # value = <UrxProbeType.LINEAR: 0>
    MATRIX: typing.ClassVar[UrxProbeType]  # value = <UrxProbeType.MATRIX: 3>
    RCA: typing.ClassVar[UrxProbeType]  # value = <UrxProbeType.RCA: 2>
    SPARSE: typing.ClassVar[UrxProbeType]  # value = <UrxProbeType.SPARSE: 4>
    UNDEFINED: typing.ClassVar[UrxProbeType]  # value = <UrxProbeType.UNDEFINED: -1>
    __members__: typing.ClassVar[dict[str, UrxProbeType]]  # value = {'LINEAR': <UrxProbeType.LINEAR: 0>, 'CURVILINEAR': <UrxProbeType.CURVILINEAR: 1>, 'RCA': <UrxProbeType.RCA: 2>, 'MATRIX': <UrxProbeType.MATRIX: 3>, 'SPARSE': <UrxProbeType.SPARSE: 4>, 'UNDEFINED': <UrxProbeType.UNDEFINED: -1>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class UrxReceiveSetup:
    __hash__: typing.ClassVar[None] = None
    active_elements: VecVecUInt32
    modulation_frequency: UrxDoubleNan
    number_samples: int
    probe: UrxProbe
    probe_transform: UrxTransform
    sampling_frequency: UrxDoubleNan
    tgc_profile: VecFloat64
    tgc_sampling_frequency: UrxDoubleNan
    time_offset: UrxDoubleNan
    def __eq__(self, arg0: UrxReceiveSetup) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxReceiveSetup) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxProbe, arg1: UrxTransform, arg2: UrxDoubleNan, arg3: int, arg4: VecVecUInt32, arg5: VecFloat64, arg6: UrxDoubleNan, arg7: UrxDoubleNan, arg8: UrxDoubleNan) -> None:
        ...
    def __ne__(self, arg0: UrxReceiveSetup) -> bool:
        ...
class UrxSamplingType:
    """
    Members:
    
      RF
    
      IQ
    
      UNDEFINED
    """
    IQ: typing.ClassVar[UrxSamplingType]  # value = <UrxSamplingType.IQ: 1>
    RF: typing.ClassVar[UrxSamplingType]  # value = <UrxSamplingType.RF: 0>
    UNDEFINED: typing.ClassVar[UrxSamplingType]  # value = <UrxSamplingType.UNDEFINED: -1>
    __members__: typing.ClassVar[dict[str, UrxSamplingType]]  # value = {'RF': <UrxSamplingType.RF: 0>, 'IQ': <UrxSamplingType.IQ: 1>, 'UNDEFINED': <UrxSamplingType.UNDEFINED: -1>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class UrxTransform:
    __hash__: typing.ClassVar[None] = None
    rotation: UrxVector3D
    translation: UrxVector3D
    def __eq__(self, arg0: UrxTransform) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxTransform) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxVector3D, arg1: UrxVector3D) -> None:
        ...
    def __ne__(self, arg0: UrxTransform) -> bool:
        ...
class UrxTransmitSetup:
    __hash__: typing.ClassVar[None] = None
    active_elements: VecVecUInt32
    delays: VecFloat64
    excitations: VecExcitationPtr
    probe: UrxProbe
    probe_transform: UrxTransform
    time_offset: UrxDoubleNan
    wave: UrxWave
    def __eq__(self, arg0: UrxTransmitSetup) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxTransmitSetup) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxProbe, arg1: UrxWave, arg2: VecVecUInt32, arg3: VecExcitationPtr, arg4: VecFloat64, arg5: UrxTransform, arg6: UrxDoubleNan) -> None:
        ...
    def __ne__(self, arg0: UrxTransmitSetup) -> bool:
        ...
class UrxVector2D:
    __hash__: typing.ClassVar[None] = None
    x: float
    y: float
    def __eq__(self, arg0: UrxVector2D) -> bool:
        ...
    @typing.overload
    def __init__(self, arg0: float, arg1: float) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxVector2D) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    def __ne__(self, arg0: UrxVector2D) -> bool:
        ...
class UrxVector3D:
    __hash__: typing.ClassVar[None] = None
    x: float
    y: float
    z: float
    def __eq__(self, arg0: UrxVector3D) -> bool:
        ...
    @typing.overload
    def __init__(self, arg0: float, arg1: float, arg2: float) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxVector3D) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    def __ne__(self, arg0: UrxVector3D) -> bool:
        ...
class UrxVersion:
    __hash__: typing.ClassVar[None] = None
    major: int
    minor: int
    patch: int
    def __eq__(self, arg0: UrxVersion) -> bool:
        ...
    @typing.overload
    def __init__(self, arg0: int, arg1: int, arg2: int) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxVersion) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    def __ne__(self, arg0: UrxVersion) -> bool:
        ...
class UrxWave:
    __hash__: typing.ClassVar[None] = None
    parameters: VecFloat64
    time_zero: UrxDoubleNan
    time_zero_reference_point: UrxVector3D
    type: UrxWaveType
    def __eq__(self, arg0: UrxWave) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxWave) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: UrxWaveType, arg1: UrxDoubleNan, arg2: UrxVector3D, arg3: VecFloat64) -> None:
        ...
    def __ne__(self, arg0: UrxWave) -> bool:
        ...
class UrxWaveType:
    """
    Members:
    
      CONVERGING_WAVE
    
      DIVERGING_WAVE
    
      PLANE_WAVE
    
      CYLINDRICAL_WAVE
    
      UNDEFINED
    """
    CONVERGING_WAVE: typing.ClassVar[UrxWaveType]  # value = <UrxWaveType.CONVERGING_WAVE: 0>
    CYLINDRICAL_WAVE: typing.ClassVar[UrxWaveType]  # value = <UrxWaveType.CYLINDRICAL_WAVE: 3>
    DIVERGING_WAVE: typing.ClassVar[UrxWaveType]  # value = <UrxWaveType.DIVERGING_WAVE: 1>
    PLANE_WAVE: typing.ClassVar[UrxWaveType]  # value = <UrxWaveType.PLANE_WAVE: 2>
    UNDEFINED: typing.ClassVar[UrxWaveType]  # value = <UrxWaveType.UNDEFINED: -1>
    __members__: typing.ClassVar[dict[str, UrxWaveType]]  # value = {'CONVERGING_WAVE': <UrxWaveType.CONVERGING_WAVE: 0>, 'DIVERGING_WAVE': <UrxWaveType.DIVERGING_WAVE: 1>, 'PLANE_WAVE': <UrxWaveType.PLANE_WAVE: 2>, 'CYLINDRICAL_WAVE': <UrxWaveType.CYLINDRICAL_WAVE: 3>, 'UNDEFINED': <UrxWaveType.UNDEFINED: -1>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class VecElement:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: UrxElement) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecElement) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecElement:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> UrxElement:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecElement) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[UrxElement]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecElement) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: UrxElement) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecElement) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: UrxElement) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: UrxElement) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecElement) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: UrxElement) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> UrxElement:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> UrxElement:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: UrxElement) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecElementGeometryPtr:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: UrxElementGeometry) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecElementGeometryPtr) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecElementGeometryPtr:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> UrxElementGeometry:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecElementGeometryPtr) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[UrxElementGeometry]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecElementGeometryPtr) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: UrxElementGeometry) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecElementGeometryPtr) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: UrxElementGeometry) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: UrxElementGeometry) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecElementGeometryPtr) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: UrxElementGeometry) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> UrxElementGeometry:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> UrxElementGeometry:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: UrxElementGeometry) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecEvent:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: UrxEvent) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecEvent) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecEvent:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> UrxEvent:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecEvent) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[UrxEvent]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecEvent) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: UrxEvent) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecEvent) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: UrxEvent) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: UrxEvent) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecEvent) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: UrxEvent) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> UrxEvent:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> UrxEvent:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: UrxEvent) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecExcitationPtr:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: UrxExcitation) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecExcitationPtr) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecExcitationPtr:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> UrxExcitation:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecExcitationPtr) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[UrxExcitation]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecExcitationPtr) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: UrxExcitation) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecExcitationPtr) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: UrxExcitation) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: UrxExcitation) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecExcitationPtr) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: UrxExcitation) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> UrxExcitation:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> UrxExcitation:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: UrxExcitation) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecFloat32:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: float) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecFloat32) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecFloat32:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> float:
        ...
    @typing.overload
    def __init__(self, arg0: typing_extensions.Buffer) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecFloat32) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[float]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecFloat32) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: float) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecFloat32) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: float) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: float) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecFloat32) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: float) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> float:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> float:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: float) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecFloat64:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: float) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecFloat64) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecFloat64:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> float:
        ...
    @typing.overload
    def __init__(self, arg0: typing_extensions.Buffer) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecFloat64) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[float]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecFloat64) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: float) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecFloat64) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: float) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: float) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecFloat64) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: float) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> float:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> float:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: float) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecGroupData:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: UrxGroupData) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecGroupData) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecGroupData:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> UrxGroupData:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecGroupData) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[UrxGroupData]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecGroupData) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: UrxGroupData) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecGroupData) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: UrxGroupData) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: UrxGroupData) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecGroupData) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: UrxGroupData) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> UrxGroupData:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> UrxGroupData:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: UrxGroupData) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecGroupPtr:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: UrxGroup) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecGroupPtr) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecGroupPtr:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> UrxGroup:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecGroupPtr) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[UrxGroup]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecGroupPtr) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: UrxGroup) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecGroupPtr) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: UrxGroup) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: UrxGroup) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecGroupPtr) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: UrxGroup) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> UrxGroup:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> UrxGroup:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: UrxGroup) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecImpulseResponsePtr:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: UrxImpulseResponse) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecImpulseResponsePtr) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecImpulseResponsePtr:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> UrxImpulseResponse:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecImpulseResponsePtr) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[UrxImpulseResponse]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecImpulseResponsePtr) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: UrxImpulseResponse) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecImpulseResponsePtr) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: UrxImpulseResponse) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: UrxImpulseResponse) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecImpulseResponsePtr) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: UrxImpulseResponse) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> UrxImpulseResponse:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> UrxImpulseResponse:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: UrxImpulseResponse) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecInt16:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecInt16) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecInt16:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> int:
        ...
    @typing.overload
    def __init__(self, arg0: typing_extensions.Buffer) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecInt16) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[int]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecInt16) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecInt16) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: int) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: int) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecInt16) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: int) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> int:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: int) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecInt32:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecInt32) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecInt32:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> int:
        ...
    @typing.overload
    def __init__(self, arg0: typing_extensions.Buffer) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecInt32) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[int]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecInt32) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecInt32) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: int) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: int) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecInt32) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: int) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> int:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: int) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecInt64:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecInt64) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecInt64:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> int:
        ...
    @typing.overload
    def __init__(self, arg0: typing_extensions.Buffer) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecInt64) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[int]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecInt64) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecInt64) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: int) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: int) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecInt64) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: int) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> int:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: int) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecInt8:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecInt8) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecInt8:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> int:
        ...
    @typing.overload
    def __init__(self, arg0: typing_extensions.Buffer) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecInt8) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[int]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecInt8) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecInt8) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: int) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: int) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecInt8) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: int) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> int:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: int) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecProbePtr:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: UrxProbe) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecProbePtr) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecProbePtr:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> UrxProbe:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecProbePtr) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[UrxProbe]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecProbePtr) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: UrxProbe) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecProbePtr) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: UrxProbe) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: UrxProbe) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecProbePtr) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: UrxProbe) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> UrxProbe:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> UrxProbe:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: UrxProbe) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecString:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: str) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecString) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecString:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> str:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecString) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[str]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecString) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: str) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecString) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: str) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: str) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecString) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: str) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> str:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> str:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: str) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecUInt16:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecUInt16) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecUInt16:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> int:
        ...
    @typing.overload
    def __init__(self, arg0: typing_extensions.Buffer) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecUInt16) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[int]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecUInt16) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecUInt16) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: int) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: int) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecUInt16) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: int) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> int:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: int) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecUInt32:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecUInt32) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecUInt32:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> int:
        ...
    @typing.overload
    def __init__(self, arg0: typing_extensions.Buffer) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecUInt32) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[int]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecUInt32) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecUInt32) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: int) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: int) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecUInt32) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: int) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> int:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: int) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecUInt64:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecUInt64) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecUInt64:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> int:
        ...
    @typing.overload
    def __init__(self, arg0: typing_extensions.Buffer) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecUInt64) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[int]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecUInt64) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecUInt64) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: int) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: int) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecUInt64) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: int) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> int:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: int) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecUInt8:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecUInt8) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecUInt8:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> int:
        ...
    @typing.overload
    def __init__(self, arg0: typing_extensions.Buffer) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecUInt8) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[int]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecUInt8) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecUInt8) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: int) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: int) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecUInt8) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: int) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> int:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: int) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecFloat32:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecFloat32) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecFloat32) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecFloat32:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecFloat32:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecFloat32) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecFloat32]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecFloat32) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecFloat32) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecFloat32) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecFloat32) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecFloat32) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecFloat32) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecFloat32) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecFloat32:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecFloat32:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecFloat32) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecFloat64:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecFloat64) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecFloat64) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecFloat64:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecFloat64:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecFloat64) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecFloat64]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecFloat64) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecFloat64) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecFloat64) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecFloat64) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecFloat64) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecFloat64) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecFloat64) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecFloat64:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecFloat64:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecFloat64) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecInt16:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecInt16) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecInt16) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecInt16:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecInt16:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecInt16) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecInt16]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecInt16) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecInt16) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecInt16) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecInt16) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecInt16) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecInt16) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecInt16) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecInt16:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecInt16:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecInt16) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecInt32:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecInt32) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecInt32) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecInt32:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecInt32:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecInt32) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecInt32]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecInt32) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecInt32) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecInt32) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecInt32) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecInt32) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecInt32) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecInt32) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecInt32:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecInt32:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecInt32) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecInt64:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecInt64) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecInt64) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecInt64:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecInt64:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecInt64) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecInt64]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecInt64) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecInt64) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecInt64) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecInt64) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecInt64) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecInt64) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecInt64) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecInt64:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecInt64:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecInt64) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecInt8:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecInt8) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecInt8) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecInt8:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecInt8:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecInt8) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecInt8]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecInt8) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecInt8) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecInt8) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecInt8) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecInt8) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecInt8) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecInt8) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecInt8:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecInt8:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecInt8) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecString:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecString) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecString) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecString:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecString:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecString) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecString]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecString) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecString) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecString) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecString) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecString) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecString) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecString) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecString:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecString:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecString) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecUInt16:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecUInt16) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecUInt16) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecUInt16:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecUInt16:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecUInt16) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecUInt16]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecUInt16) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecUInt16) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecUInt16) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecUInt16) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecUInt16) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecUInt16) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecUInt16) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecUInt16:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecUInt16:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecUInt16) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecUInt32:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecUInt32) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecUInt32) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecUInt32:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecUInt32:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecUInt32) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecUInt32]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecUInt32) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecUInt32) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecUInt32) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecUInt32) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecUInt32) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecUInt32) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecUInt32) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecUInt32:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecUInt32:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecUInt32) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecUInt64:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecUInt64) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecUInt64) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecUInt64:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecUInt64:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecUInt64) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecUInt64]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecUInt64) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecUInt64) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecUInt64) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecUInt64) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecUInt64) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecUInt64) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecUInt64) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecUInt64:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecUInt64:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecUInt64) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVecUInt8:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: VecUInt8) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVecUInt8) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVecUInt8:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> VecUInt8:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVecUInt8) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[VecUInt8]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVecUInt8) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: VecUInt8) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVecUInt8) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: VecUInt8) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: VecUInt8) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVecUInt8) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: VecUInt8) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> VecUInt8:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> VecUInt8:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: VecUInt8) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVector2D:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: UrxVector2D) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVector2D) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVector2D:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> UrxVector2D:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVector2D) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[UrxVector2D]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVector2D) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: UrxVector2D) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVector2D) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: UrxVector2D) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: UrxVector2D) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVector2D) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: UrxVector2D) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> UrxVector2D:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> UrxVector2D:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: UrxVector2D) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecVector3D:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: UrxVector3D) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self, arg0: VecVector3D) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> VecVector3D:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: int) -> UrxVector3D:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: VecVector3D) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None:
        ...
    def __iter__(self) -> typing.Iterator[UrxVector3D]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVector3D) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: UrxVector3D) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVector3D) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: UrxVector3D) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: UrxVector3D) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: VecVector3D) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: int, x: UrxVector3D) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> UrxVector3D:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> UrxVector3D:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: UrxVector3D) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
def isIso3166(arg0: str) -> bool:
    ...
def isIso8601(arg0: str) -> bool:
    ...
def loadFromFile(arg0: str) -> UrxDataset:
    ...
def pyGetFormat(arg0: UrxDataType) -> str:
    ...
def saveToFile(arg0: str, arg1: UrxDataset) -> None:
    ...
def sizeofDataType(arg0: UrxDataType) -> int:
    ...
def sizeofSample(arg0: UrxSamplingType, arg1: UrxDataType) -> int:
    ...
URX_VERSION_MAJOR: int = 1
URX_VERSION_MINOR: int = 0
URX_VERSION_PATCH: int = 0
Acquisition = UrxAcquisition
DataType = UrxDataType
Dataset = UrxDataset
DoubleNan = UrxDoubleNan
Element = UrxElement
ElementGeometry = UrxElementGeometry
Event = UrxEvent
Excitation = UrxExcitation
Group = UrxGroup
GroupData = UrxGroupData
ImpulseResponse = UrxImpulseResponse
Probe = UrxProbe
ProbeType = UrxProbeType
ReceiveSetup = UrxReceiveSetup
SamplingType = UrxSamplingType
Transform = UrxTransform
TransmitSetup = UrxTransmitSetup
Vector2D = UrxVector2D
Vector3D = UrxVector3D
Version = UrxVersion
Wave = UrxWave
WaveType = UrxWaveType
