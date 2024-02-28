"""
Python binding of urx library
"""
from __future__ import annotations
import numpy
import typing
import typing_extensions
__all__ = ['Acquisition', 'DataType', 'Dataset', 'DoubleNan', 'Element', 'ElementGeometry', 'Event', 'Excitation', 'Group', 'GroupData', 'GroupDataReader', 'ImpulseResponse', 'Probe', 'ProbeType', 'ReceiveSetup', 'SamplingType', 'Transform', 'TransmitSetup', 'URX_VERSION_MAJOR', 'URX_VERSION_MINOR', 'URX_VERSION_PATCH', 'VecElement', 'VecElementGeometryPtr', 'VecEvent', 'VecExcitationPtr', 'VecFloat32', 'VecFloat64', 'VecGroupData', 'VecGroupPtr', 'VecImpulseResponsePtr', 'VecInt16', 'VecInt32', 'VecInt64', 'VecInt8', 'VecProbePtr', 'VecString', 'VecUInt16', 'VecUInt32', 'VecUInt64', 'VecUInt8', 'VecVecFloat64', 'VecVecUInt32', 'VecVector3D', 'Vector2D', 'Vector3D', 'Version', 'Wave', 'WaveType', 'isIso3166', 'isIso8601', 'loadFromFile', 'pyGetFormat', 'saveToFile', 'sizeofDataType', 'sizeofSample']
class Acquisition:
    __hash__: typing.ClassVar[None] = None
    authors: str
    country_code: str
    description: str
    excitations: VecExcitationPtr
    groups: VecGroupPtr
    groups_data: VecGroupData
    local_time: str
    probes: VecProbePtr
    sound_speed: DoubleNan
    system: str
    timestamp: DoubleNan
    def __eq__(self, arg0: Acquisition) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Acquisition) -> None:
        ...
    def __ne__(self, arg0: Acquisition) -> bool:
        ...
class DataType:
    """
    Members:
    
      INT16
    
      INT32
    
      FLOAT
    
      DOUBLE
    
      UNDEFINED
    """
    DOUBLE: typing.ClassVar[DataType]  # value = <DataType.DOUBLE: 3>
    FLOAT: typing.ClassVar[DataType]  # value = <DataType.FLOAT: 2>
    INT16: typing.ClassVar[DataType]  # value = <DataType.INT16: 0>
    INT32: typing.ClassVar[DataType]  # value = <DataType.INT32: 1>
    UNDEFINED: typing.ClassVar[DataType]  # value = <DataType.UNDEFINED: -1>
    __members__: typing.ClassVar[dict[str, DataType]]  # value = {'INT16': <DataType.INT16: 0>, 'INT32': <DataType.INT32: 1>, 'FLOAT': <DataType.FLOAT: 2>, 'DOUBLE': <DataType.DOUBLE: 3>, 'UNDEFINED': <DataType.UNDEFINED: -1>}
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
class Dataset:
    __hash__: typing.ClassVar[None] = None
    acquisition: Acquisition
    version: Version
    def __eq__(self, arg0: Dataset) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Dataset) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Acquisition, arg1: Version) -> None:
        ...
    def __ne__(self, arg0: Dataset) -> bool:
        ...
class DoubleNan:
    __hash__: typing.ClassVar[None] = None
    value: float
    @typing.overload
    def __add__(self, arg0: DoubleNan) -> DoubleNan:
        ...
    @typing.overload
    def __add__(self, arg0: float) -> DoubleNan:
        ...
    @typing.overload
    def __eq__(self, arg0: DoubleNan) -> bool:
        ...
    @typing.overload
    def __eq__(self, arg0: float) -> bool:
        ...
    @typing.overload
    def __iadd__(self, arg0: DoubleNan) -> DoubleNan:
        ...
    @typing.overload
    def __iadd__(self, arg0: float) -> DoubleNan:
        ...
    @typing.overload
    def __imul__(self, arg0: DoubleNan) -> DoubleNan:
        ...
    @typing.overload
    def __imul__(self, arg0: float) -> DoubleNan:
        ...
    @typing.overload
    def __init__(self, arg0: float) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: DoubleNan) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __isub__(self, arg0: DoubleNan) -> DoubleNan:
        ...
    @typing.overload
    def __isub__(self, arg0: float) -> DoubleNan:
        ...
    @typing.overload
    def __itruediv__(self, arg0: DoubleNan) -> DoubleNan:
        ...
    @typing.overload
    def __itruediv__(self, arg0: float) -> DoubleNan:
        ...
    @typing.overload
    def __mul__(self, arg0: DoubleNan) -> DoubleNan:
        ...
    @typing.overload
    def __mul__(self, arg0: float) -> DoubleNan:
        ...
    @typing.overload
    def __ne__(self, arg0: DoubleNan) -> bool:
        ...
    @typing.overload
    def __ne__(self, arg0: float) -> bool:
        ...
    def __neg__(self) -> float:
        ...
    def __pos__(self) -> float:
        ...
    def __radd__(self, arg0: float) -> DoubleNan:
        ...
    def __rmul__(self, arg0: float) -> DoubleNan:
        ...
    def __rsub__(self, arg0: float) -> DoubleNan:
        ...
    def __rtruediv__(self, arg0: float) -> DoubleNan:
        ...
    @typing.overload
    def __sub__(self, arg0: DoubleNan) -> DoubleNan:
        ...
    @typing.overload
    def __sub__(self, arg0: float) -> DoubleNan:
        ...
    @typing.overload
    def __truediv__(self, arg0: DoubleNan) -> DoubleNan:
        ...
    @typing.overload
    def __truediv__(self, arg0: float) -> DoubleNan:
        ...
class Element:
    __hash__: typing.ClassVar[None] = None
    element_geometry: ElementGeometry
    impulse_response: ImpulseResponse
    transform: Transform
    def __eq__(self, arg0: Element) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Element) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Transform, arg1: ElementGeometry, arg2: ImpulseResponse) -> None:
        ...
    def __ne__(self, arg0: Element) -> bool:
        ...
class ElementGeometry:
    __hash__: typing.ClassVar[None] = None
    perimeter: VecVector3D
    def __eq__(self, arg0: ElementGeometry) -> bool:
        ...
    @typing.overload
    def __init__(self, arg0: VecVector3D) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: ElementGeometry) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    def __ne__(self, arg0: ElementGeometry) -> bool:
        ...
class Event:
    __hash__: typing.ClassVar[None] = None
    receive_setup: ReceiveSetup
    transmit_setup: TransmitSetup
    def __eq__(self, arg0: Event) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Event) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: TransmitSetup, arg1: ReceiveSetup) -> None:
        ...
    def __ne__(self, arg0: Event) -> bool:
        ...
class Excitation:
    __hash__: typing.ClassVar[None] = None
    pulse_shape: str
    sampling_frequency: DoubleNan
    transmit_frequency: DoubleNan
    waveform: VecFloat64
    def __eq__(self, arg0: Excitation) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Excitation) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: str, arg1: DoubleNan, arg2: DoubleNan, arg3: VecFloat64) -> None:
        ...
    def __ne__(self, arg0: Excitation) -> bool:
        ...
class Group:
    __hash__: typing.ClassVar[None] = None
    data_type: DataType
    description: str
    sampling_type: SamplingType
    sequence: VecEvent
    def __eq__(self, arg0: Group) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Group) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: SamplingType, arg1: DataType, arg2: str, arg3: VecEvent) -> None:
        ...
    def __ne__(self, arg0: Group) -> bool:
        ...
class GroupData:
    __hash__: typing.ClassVar[None] = None
    event_timestamps: VecVecFloat64
    group: Group
    group_timestamp: DoubleNan
    sequence_timestamps: VecFloat64
    def __eq__(self, arg0: GroupData) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: GroupData) -> None:
        ...
    def __ne__(self, arg0: GroupData) -> bool:
        ...
    @property
    def raw_data(self) -> numpy.ndarray:
        ...
    @raw_data.setter
    def raw_data(self, arg1: typing_extensions.Buffer) -> None:
        ...
class GroupDataReader:
    def __init__(self, arg0: GroupData, arg1: int) -> None:
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
class ImpulseResponse:
    __hash__: typing.ClassVar[None] = None
    data: VecFloat64
    sampling_frequency: DoubleNan
    time_offset: DoubleNan
    units: str
    def __eq__(self, arg0: ImpulseResponse) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: ImpulseResponse) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: DoubleNan, arg1: DoubleNan, arg2: str, arg3: VecFloat64) -> None:
        ...
    def __ne__(self, arg0: ImpulseResponse) -> bool:
        ...
class Probe:
    __hash__: typing.ClassVar[None] = None
    description: str
    element_geometries: VecElementGeometryPtr
    elements: VecElement
    impulse_responses: VecImpulseResponsePtr
    transform: Transform
    type: ProbeType
    def __eq__(self, arg0: Probe) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Probe) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: str, arg1: ProbeType, arg2: Transform, arg3: VecElementGeometryPtr, arg4: VecImpulseResponsePtr, arg5: VecElement) -> None:
        ...
    def __ne__(self, arg0: Probe) -> bool:
        ...
class ProbeType:
    """
    Members:
    
      LINEAR
    
      CURVILINEAR
    
      RCA
    
      MATRIX
    
      SPARSE
    
      UNDEFINED
    """
    CURVILINEAR: typing.ClassVar[ProbeType]  # value = <ProbeType.CURVILINEAR: 1>
    LINEAR: typing.ClassVar[ProbeType]  # value = <ProbeType.LINEAR: 0>
    MATRIX: typing.ClassVar[ProbeType]  # value = <ProbeType.MATRIX: 3>
    RCA: typing.ClassVar[ProbeType]  # value = <ProbeType.RCA: 2>
    SPARSE: typing.ClassVar[ProbeType]  # value = <ProbeType.SPARSE: 4>
    UNDEFINED: typing.ClassVar[ProbeType]  # value = <ProbeType.UNDEFINED: -1>
    __members__: typing.ClassVar[dict[str, ProbeType]]  # value = {'LINEAR': <ProbeType.LINEAR: 0>, 'CURVILINEAR': <ProbeType.CURVILINEAR: 1>, 'RCA': <ProbeType.RCA: 2>, 'MATRIX': <ProbeType.MATRIX: 3>, 'SPARSE': <ProbeType.SPARSE: 4>, 'UNDEFINED': <ProbeType.UNDEFINED: -1>}
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
class ReceiveSetup:
    __hash__: typing.ClassVar[None] = None
    active_elements: VecVecUInt32
    modulation_frequency: DoubleNan
    number_samples: int
    probe: Probe
    probe_transform: Transform
    sampling_frequency: DoubleNan
    tgc_profile: VecFloat64
    tgc_sampling_frequency: DoubleNan
    time_offset: DoubleNan
    def __eq__(self, arg0: ReceiveSetup) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: ReceiveSetup) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Probe, arg1: Transform, arg2: DoubleNan, arg3: int, arg4: VecVecUInt32, arg5: VecFloat64, arg6: DoubleNan, arg7: DoubleNan, arg8: DoubleNan) -> None:
        ...
    def __ne__(self, arg0: ReceiveSetup) -> bool:
        ...
class SamplingType:
    """
    Members:
    
      RF
    
      IQ
    
      UNDEFINED
    """
    IQ: typing.ClassVar[SamplingType]  # value = <SamplingType.IQ: 1>
    RF: typing.ClassVar[SamplingType]  # value = <SamplingType.RF: 0>
    UNDEFINED: typing.ClassVar[SamplingType]  # value = <SamplingType.UNDEFINED: -1>
    __members__: typing.ClassVar[dict[str, SamplingType]]  # value = {'RF': <SamplingType.RF: 0>, 'IQ': <SamplingType.IQ: 1>, 'UNDEFINED': <SamplingType.UNDEFINED: -1>}
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
class Transform:
    __hash__: typing.ClassVar[None] = None
    rotation: Vector3D
    translation: Vector3D
    def __eq__(self, arg0: Transform) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Vector3D, arg1: Vector3D) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Transform) -> None:
        ...
    def __ne__(self, arg0: Transform) -> bool:
        ...
class TransmitSetup:
    __hash__: typing.ClassVar[None] = None
    active_elements: VecVecUInt32
    delays: VecFloat64
    excitations: VecExcitationPtr
    probe: Probe
    probe_transform: Transform
    time_offset: DoubleNan
    wave: Wave
    def __eq__(self, arg0: TransmitSetup) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: TransmitSetup) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Probe, arg1: Wave, arg2: VecVecUInt32, arg3: VecExcitationPtr, arg4: VecFloat64, arg5: Transform, arg6: DoubleNan) -> None:
        ...
    def __ne__(self, arg0: TransmitSetup) -> bool:
        ...
class VecElement:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: Element) -> bool:
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
    def __getitem__(self, arg0: int) -> Element:
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
    def __iter__(self) -> typing.Iterator:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecElement) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: Element) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecElement) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: Element) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: Element) -> int:
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
    def insert(self, i: int, x: Element) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> Element:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> Element:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: Element) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecElementGeometryPtr:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: ElementGeometry) -> bool:
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
    def __getitem__(self, arg0: int) -> ElementGeometry:
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
    def __iter__(self) -> typing.Iterator:
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
    def __setitem__(self, arg0: int, arg1: ElementGeometry) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecElementGeometryPtr) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: ElementGeometry) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: ElementGeometry) -> int:
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
    def insert(self, i: int, x: ElementGeometry) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> ElementGeometry:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> ElementGeometry:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: ElementGeometry) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecEvent:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: Event) -> bool:
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
    def __getitem__(self, arg0: int) -> Event:
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
    def __iter__(self) -> typing.Iterator:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecEvent) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: Event) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecEvent) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: Event) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: Event) -> int:
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
    def insert(self, i: int, x: Event) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> Event:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> Event:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: Event) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecExcitationPtr:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: Excitation) -> bool:
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
    def __getitem__(self, arg0: int) -> Excitation:
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
    def __iter__(self) -> typing.Iterator:
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
    def __setitem__(self, arg0: int, arg1: Excitation) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecExcitationPtr) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: Excitation) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: Excitation) -> int:
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
    def insert(self, i: int, x: Excitation) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> Excitation:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> Excitation:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: Excitation) -> None:
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
    def __iter__(self) -> typing.Iterator:
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
    def __iter__(self) -> typing.Iterator:
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
    def __contains__(self, x: GroupData) -> bool:
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
    def __getitem__(self, arg0: int) -> GroupData:
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
    def __iter__(self) -> typing.Iterator:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecGroupData) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: GroupData) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecGroupData) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: GroupData) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: GroupData) -> int:
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
    def insert(self, i: int, x: GroupData) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> GroupData:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> GroupData:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: GroupData) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecGroupPtr:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: Group) -> bool:
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
    def __getitem__(self, arg0: int) -> Group:
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
    def __iter__(self) -> typing.Iterator:
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
    def __setitem__(self, arg0: int, arg1: Group) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecGroupPtr) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: Group) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: Group) -> int:
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
    def insert(self, i: int, x: Group) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> Group:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> Group:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: Group) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class VecImpulseResponsePtr:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: ImpulseResponse) -> bool:
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
    def __getitem__(self, arg0: int) -> ImpulseResponse:
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
    def __iter__(self) -> typing.Iterator:
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
    def __setitem__(self, arg0: int, arg1: ImpulseResponse) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecImpulseResponsePtr) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: ImpulseResponse) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: ImpulseResponse) -> int:
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
    def insert(self, i: int, x: ImpulseResponse) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> ImpulseResponse:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> ImpulseResponse:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: ImpulseResponse) -> None:
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
    def __iter__(self) -> typing.Iterator:
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
    def __iter__(self) -> typing.Iterator:
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
    def __iter__(self) -> typing.Iterator:
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
    def __iter__(self) -> typing.Iterator:
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
    def __contains__(self, x: Probe) -> bool:
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
    def __getitem__(self, arg0: int) -> Probe:
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
    def __iter__(self) -> typing.Iterator:
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
    def __setitem__(self, arg0: int, arg1: Probe) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecProbePtr) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: Probe) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: Probe) -> int:
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
    def insert(self, i: int, x: Probe) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> Probe:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> Probe:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: Probe) -> None:
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
    def __iter__(self) -> typing.Iterator:
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
    def __iter__(self) -> typing.Iterator:
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
    def __iter__(self) -> typing.Iterator:
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
    def __iter__(self) -> typing.Iterator:
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
    def __iter__(self) -> typing.Iterator:
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
    def __iter__(self) -> typing.Iterator:
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
    def __iter__(self) -> typing.Iterator:
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
class VecVector3D:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: Vector3D) -> bool:
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
    def __getitem__(self, arg0: int) -> Vector3D:
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
    def __iter__(self) -> typing.Iterator:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: VecVector3D) -> bool:
        ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: Vector3D) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: VecVector3D) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: Vector3D) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: Vector3D) -> int:
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
    def insert(self, i: int, x: Vector3D) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> Vector3D:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: int) -> Vector3D:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: Vector3D) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class Vector2D:
    __hash__: typing.ClassVar[None] = None
    x: float
    y: float
    def __eq__(self, arg0: Vector2D) -> bool:
        ...
    @typing.overload
    def __init__(self, arg0: float, arg1: float) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Vector2D) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    def __ne__(self, arg0: Vector2D) -> bool:
        ...
class Vector3D:
    __hash__: typing.ClassVar[None] = None
    x: float
    y: float
    z: float
    def __eq__(self, arg0: Vector3D) -> bool:
        ...
    @typing.overload
    def __init__(self, arg0: float, arg1: float, arg2: float) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Vector3D) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    def __ne__(self, arg0: Vector3D) -> bool:
        ...
class Version:
    __hash__: typing.ClassVar[None] = None
    major: int
    minor: int
    patch: int
    def __eq__(self, arg0: Version) -> bool:
        ...
    @typing.overload
    def __init__(self, arg0: int, arg1: int, arg2: int) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Version) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    def __ne__(self, arg0: Version) -> bool:
        ...
class Wave:
    __hash__: typing.ClassVar[None] = None
    parameters: VecFloat64
    time_zero: DoubleNan
    time_zero_reference_point: Vector3D
    type: WaveType
    def __eq__(self, arg0: Wave) -> bool:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: Wave) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: WaveType, arg1: DoubleNan, arg2: Vector3D, arg3: VecFloat64) -> None:
        ...
    def __ne__(self, arg0: Wave) -> bool:
        ...
class WaveType:
    """
    Members:
    
      CONVERGING_WAVE
    
      DIVERGING_WAVE
    
      PLANE_WAVE
    
      CYLINDRICAL_WAVE
    
      UNDEFINED
    """
    CONVERGING_WAVE: typing.ClassVar[WaveType]  # value = <WaveType.CONVERGING_WAVE: 0>
    CYLINDRICAL_WAVE: typing.ClassVar[WaveType]  # value = <WaveType.CYLINDRICAL_WAVE: 3>
    DIVERGING_WAVE: typing.ClassVar[WaveType]  # value = <WaveType.DIVERGING_WAVE: 1>
    PLANE_WAVE: typing.ClassVar[WaveType]  # value = <WaveType.PLANE_WAVE: 2>
    UNDEFINED: typing.ClassVar[WaveType]  # value = <WaveType.UNDEFINED: -1>
    __members__: typing.ClassVar[dict[str, WaveType]]  # value = {'CONVERGING_WAVE': <WaveType.CONVERGING_WAVE: 0>, 'DIVERGING_WAVE': <WaveType.DIVERGING_WAVE: 1>, 'PLANE_WAVE': <WaveType.PLANE_WAVE: 2>, 'CYLINDRICAL_WAVE': <WaveType.CYLINDRICAL_WAVE: 3>, 'UNDEFINED': <WaveType.UNDEFINED: -1>}
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
def isIso3166(arg0: str) -> bool:
    ...
def isIso8601(arg0: str) -> bool:
    ...
def loadFromFile(arg0: str) -> Dataset:
    ...
def pyGetFormat(arg0: DataType) -> str:
    ...
def saveToFile(arg0: str, arg1: Dataset) -> None:
    ...
def sizeofDataType(arg0: DataType) -> int:
    ...
def sizeofSample(arg0: SamplingType, arg1: DataType) -> int:
    ...
URX_VERSION_MAJOR: int = 1
URX_VERSION_MINOR: int = 0
URX_VERSION_PATCH: int = 0
