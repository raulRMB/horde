// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_EVENTS_NET_H_
#define FLATBUFFERS_GENERATED_EVENTS_NET_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 &&
              FLATBUFFERS_VERSION_MINOR == 5 &&
              FLATBUFFERS_VERSION_REVISION == 26,
             "Non-compatible flatbuffers version included");

namespace Net {

struct Vector2;
struct Vector2Builder;

struct Vector3;
struct Vector3Builder;

struct Vector4;
struct Vector4Builder;

struct Transform;
struct TransformBuilder;

struct SyncTransform;
struct SyncTransformBuilder;

struct OnPlayerJoined;
struct OnPlayerJoinedBuilder;

struct OnConnectionResponse;
struct OnConnectionResponseBuilder;

struct OnConnection;
struct OnConnectionBuilder;

struct OnMoveTo;
struct OnMoveToBuilder;

struct Header;
struct HeaderBuilder;

enum Events : uint8_t {
  Events_NONE = 0,
  Events_OnConnectionResponse = 1,
  Events_OnConnection = 2,
  Events_OnMoveTo = 3,
  Events_SyncTransform = 4,
  Events_OnPlayerJoined = 5,
  Events_MIN = Events_NONE,
  Events_MAX = Events_OnPlayerJoined
};

inline const Events (&EnumValuesEvents())[6] {
  static const Events values[] = {
    Events_NONE,
    Events_OnConnectionResponse,
    Events_OnConnection,
    Events_OnMoveTo,
    Events_SyncTransform,
    Events_OnPlayerJoined
  };
  return values;
}

inline const char * const *EnumNamesEvents() {
  static const char * const names[7] = {
    "NONE",
    "OnConnectionResponse",
    "OnConnection",
    "OnMoveTo",
    "SyncTransform",
    "OnPlayerJoined",
    nullptr
  };
  return names;
}

inline const char *EnumNameEvents(Events e) {
  if (::flatbuffers::IsOutRange(e, Events_NONE, Events_OnPlayerJoined)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesEvents()[index];
}

template<typename T> struct EventsTraits {
  static const Events enum_value = Events_NONE;
};

template<> struct EventsTraits<Net::OnConnectionResponse> {
  static const Events enum_value = Events_OnConnectionResponse;
};

template<> struct EventsTraits<Net::OnConnection> {
  static const Events enum_value = Events_OnConnection;
};

template<> struct EventsTraits<Net::OnMoveTo> {
  static const Events enum_value = Events_OnMoveTo;
};

template<> struct EventsTraits<Net::SyncTransform> {
  static const Events enum_value = Events_SyncTransform;
};

template<> struct EventsTraits<Net::OnPlayerJoined> {
  static const Events enum_value = Events_OnPlayerJoined;
};

bool VerifyEvents(::flatbuffers::Verifier &verifier, const void *obj, Events type);
bool VerifyEventsVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types);

struct Vector2 FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef Vector2Builder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_X = 4,
    VT_Y = 6
  };
  float x() const {
    return GetField<float>(VT_X, 0.0f);
  }
  float y() const {
    return GetField<float>(VT_Y, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_X, 4) &&
           VerifyField<float>(verifier, VT_Y, 4) &&
           verifier.EndTable();
  }
};

struct Vector2Builder {
  typedef Vector2 Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_x(float x) {
    fbb_.AddElement<float>(Vector2::VT_X, x, 0.0f);
  }
  void add_y(float y) {
    fbb_.AddElement<float>(Vector2::VT_Y, y, 0.0f);
  }
  explicit Vector2Builder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Vector2> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Vector2>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Vector2> CreateVector2(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float x = 0.0f,
    float y = 0.0f) {
  Vector2Builder builder_(_fbb);
  builder_.add_y(y);
  builder_.add_x(x);
  return builder_.Finish();
}

struct Vector3 FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef Vector3Builder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_X = 4,
    VT_Y = 6,
    VT_Z = 8
  };
  float x() const {
    return GetField<float>(VT_X, 0.0f);
  }
  float y() const {
    return GetField<float>(VT_Y, 0.0f);
  }
  float z() const {
    return GetField<float>(VT_Z, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_X, 4) &&
           VerifyField<float>(verifier, VT_Y, 4) &&
           VerifyField<float>(verifier, VT_Z, 4) &&
           verifier.EndTable();
  }
};

struct Vector3Builder {
  typedef Vector3 Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_x(float x) {
    fbb_.AddElement<float>(Vector3::VT_X, x, 0.0f);
  }
  void add_y(float y) {
    fbb_.AddElement<float>(Vector3::VT_Y, y, 0.0f);
  }
  void add_z(float z) {
    fbb_.AddElement<float>(Vector3::VT_Z, z, 0.0f);
  }
  explicit Vector3Builder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Vector3> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Vector3>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Vector3> CreateVector3(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float x = 0.0f,
    float y = 0.0f,
    float z = 0.0f) {
  Vector3Builder builder_(_fbb);
  builder_.add_z(z);
  builder_.add_y(y);
  builder_.add_x(x);
  return builder_.Finish();
}

struct Vector4 FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef Vector4Builder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_X = 4,
    VT_Y = 6,
    VT_Z = 8,
    VT_W = 10
  };
  float x() const {
    return GetField<float>(VT_X, 0.0f);
  }
  float y() const {
    return GetField<float>(VT_Y, 0.0f);
  }
  float z() const {
    return GetField<float>(VT_Z, 0.0f);
  }
  float w() const {
    return GetField<float>(VT_W, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_X, 4) &&
           VerifyField<float>(verifier, VT_Y, 4) &&
           VerifyField<float>(verifier, VT_Z, 4) &&
           VerifyField<float>(verifier, VT_W, 4) &&
           verifier.EndTable();
  }
};

struct Vector4Builder {
  typedef Vector4 Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_x(float x) {
    fbb_.AddElement<float>(Vector4::VT_X, x, 0.0f);
  }
  void add_y(float y) {
    fbb_.AddElement<float>(Vector4::VT_Y, y, 0.0f);
  }
  void add_z(float z) {
    fbb_.AddElement<float>(Vector4::VT_Z, z, 0.0f);
  }
  void add_w(float w) {
    fbb_.AddElement<float>(Vector4::VT_W, w, 0.0f);
  }
  explicit Vector4Builder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Vector4> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Vector4>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Vector4> CreateVector4(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float x = 0.0f,
    float y = 0.0f,
    float z = 0.0f,
    float w = 0.0f) {
  Vector4Builder builder_(_fbb);
  builder_.add_w(w);
  builder_.add_z(z);
  builder_.add_y(y);
  builder_.add_x(x);
  return builder_.Finish();
}

struct Transform FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef TransformBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TRANSLATION = 4,
    VT_SCALE = 6,
    VT_QUATERNION = 8
  };
  const Net::Vector3 *translation() const {
    return GetPointer<const Net::Vector3 *>(VT_TRANSLATION);
  }
  const Net::Vector3 *scale() const {
    return GetPointer<const Net::Vector3 *>(VT_SCALE);
  }
  const Net::Vector4 *quaternion() const {
    return GetPointer<const Net::Vector4 *>(VT_QUATERNION);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TRANSLATION) &&
           verifier.VerifyTable(translation()) &&
           VerifyOffset(verifier, VT_SCALE) &&
           verifier.VerifyTable(scale()) &&
           VerifyOffset(verifier, VT_QUATERNION) &&
           verifier.VerifyTable(quaternion()) &&
           verifier.EndTable();
  }
};

struct TransformBuilder {
  typedef Transform Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_translation(::flatbuffers::Offset<Net::Vector3> translation) {
    fbb_.AddOffset(Transform::VT_TRANSLATION, translation);
  }
  void add_scale(::flatbuffers::Offset<Net::Vector3> scale) {
    fbb_.AddOffset(Transform::VT_SCALE, scale);
  }
  void add_quaternion(::flatbuffers::Offset<Net::Vector4> quaternion) {
    fbb_.AddOffset(Transform::VT_QUATERNION, quaternion);
  }
  explicit TransformBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Transform> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Transform>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Transform> CreateTransform(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<Net::Vector3> translation = 0,
    ::flatbuffers::Offset<Net::Vector3> scale = 0,
    ::flatbuffers::Offset<Net::Vector4> quaternion = 0) {
  TransformBuilder builder_(_fbb);
  builder_.add_quaternion(quaternion);
  builder_.add_scale(scale);
  builder_.add_translation(translation);
  return builder_.Finish();
}

struct SyncTransform FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef SyncTransformBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TRANSFORM = 4,
    VT_NETID = 6
  };
  const Net::Transform *transform() const {
    return GetPointer<const Net::Transform *>(VT_TRANSFORM);
  }
  uint32_t netId() const {
    return GetField<uint32_t>(VT_NETID, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TRANSFORM) &&
           verifier.VerifyTable(transform()) &&
           VerifyField<uint32_t>(verifier, VT_NETID, 4) &&
           verifier.EndTable();
  }
};

struct SyncTransformBuilder {
  typedef SyncTransform Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_transform(::flatbuffers::Offset<Net::Transform> transform) {
    fbb_.AddOffset(SyncTransform::VT_TRANSFORM, transform);
  }
  void add_netId(uint32_t netId) {
    fbb_.AddElement<uint32_t>(SyncTransform::VT_NETID, netId, 0);
  }
  explicit SyncTransformBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<SyncTransform> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<SyncTransform>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<SyncTransform> CreateSyncTransform(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<Net::Transform> transform = 0,
    uint32_t netId = 0) {
  SyncTransformBuilder builder_(_fbb);
  builder_.add_netId(netId);
  builder_.add_transform(transform);
  return builder_.Finish();
}

struct OnPlayerJoined FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef OnPlayerJoinedBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NETID = 4
  };
  uint32_t netId() const {
    return GetField<uint32_t>(VT_NETID, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_NETID, 4) &&
           verifier.EndTable();
  }
};

struct OnPlayerJoinedBuilder {
  typedef OnPlayerJoined Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_netId(uint32_t netId) {
    fbb_.AddElement<uint32_t>(OnPlayerJoined::VT_NETID, netId, 0);
  }
  explicit OnPlayerJoinedBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<OnPlayerJoined> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<OnPlayerJoined>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<OnPlayerJoined> CreateOnPlayerJoined(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t netId = 0) {
  OnPlayerJoinedBuilder builder_(_fbb);
  builder_.add_netId(netId);
  return builder_.Finish();
}

struct OnConnectionResponse FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef OnConnectionResponseBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NETID = 4,
    VT_OTHERPLAYERS = 6
  };
  int32_t netId() const {
    return GetField<int32_t>(VT_NETID, 0);
  }
  const ::flatbuffers::Vector<int32_t> *otherPlayers() const {
    return GetPointer<const ::flatbuffers::Vector<int32_t> *>(VT_OTHERPLAYERS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_NETID, 4) &&
           VerifyOffset(verifier, VT_OTHERPLAYERS) &&
           verifier.VerifyVector(otherPlayers()) &&
           verifier.EndTable();
  }
};

struct OnConnectionResponseBuilder {
  typedef OnConnectionResponse Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_netId(int32_t netId) {
    fbb_.AddElement<int32_t>(OnConnectionResponse::VT_NETID, netId, 0);
  }
  void add_otherPlayers(::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> otherPlayers) {
    fbb_.AddOffset(OnConnectionResponse::VT_OTHERPLAYERS, otherPlayers);
  }
  explicit OnConnectionResponseBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<OnConnectionResponse> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<OnConnectionResponse>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<OnConnectionResponse> CreateOnConnectionResponse(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int32_t netId = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> otherPlayers = 0) {
  OnConnectionResponseBuilder builder_(_fbb);
  builder_.add_otherPlayers(otherPlayers);
  builder_.add_netId(netId);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<OnConnectionResponse> CreateOnConnectionResponseDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int32_t netId = 0,
    const std::vector<int32_t> *otherPlayers = nullptr) {
  auto otherPlayers__ = otherPlayers ? _fbb.CreateVector<int32_t>(*otherPlayers) : 0;
  return Net::CreateOnConnectionResponse(
      _fbb,
      netId,
      otherPlayers__);
}

struct OnConnection FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef OnConnectionBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_SECRET = 4
  };
  const ::flatbuffers::String *secret() const {
    return GetPointer<const ::flatbuffers::String *>(VT_SECRET);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_SECRET) &&
           verifier.VerifyString(secret()) &&
           verifier.EndTable();
  }
};

struct OnConnectionBuilder {
  typedef OnConnection Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_secret(::flatbuffers::Offset<::flatbuffers::String> secret) {
    fbb_.AddOffset(OnConnection::VT_SECRET, secret);
  }
  explicit OnConnectionBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<OnConnection> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<OnConnection>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<OnConnection> CreateOnConnection(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> secret = 0) {
  OnConnectionBuilder builder_(_fbb);
  builder_.add_secret(secret);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<OnConnection> CreateOnConnectionDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *secret = nullptr) {
  auto secret__ = secret ? _fbb.CreateString(secret) : 0;
  return Net::CreateOnConnection(
      _fbb,
      secret__);
}

struct OnMoveTo FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef OnMoveToBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NETID = 4,
    VT_POS = 6
  };
  uint32_t netId() const {
    return GetField<uint32_t>(VT_NETID, 0);
  }
  const Net::Vector2 *pos() const {
    return GetPointer<const Net::Vector2 *>(VT_POS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_NETID, 4) &&
           VerifyOffset(verifier, VT_POS) &&
           verifier.VerifyTable(pos()) &&
           verifier.EndTable();
  }
};

struct OnMoveToBuilder {
  typedef OnMoveTo Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_netId(uint32_t netId) {
    fbb_.AddElement<uint32_t>(OnMoveTo::VT_NETID, netId, 0);
  }
  void add_pos(::flatbuffers::Offset<Net::Vector2> pos) {
    fbb_.AddOffset(OnMoveTo::VT_POS, pos);
  }
  explicit OnMoveToBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<OnMoveTo> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<OnMoveTo>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<OnMoveTo> CreateOnMoveTo(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t netId = 0,
    ::flatbuffers::Offset<Net::Vector2> pos = 0) {
  OnMoveToBuilder builder_(_fbb);
  builder_.add_pos(pos);
  builder_.add_netId(netId);
  return builder_.Finish();
}

struct Header FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef HeaderBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_EVENT_TYPE = 4,
    VT_EVENT = 6
  };
  Net::Events Event_type() const {
    return static_cast<Net::Events>(GetField<uint8_t>(VT_EVENT_TYPE, 0));
  }
  const void *Event() const {
    return GetPointer<const void *>(VT_EVENT);
  }
  template<typename T> const T *Event_as() const;
  const Net::OnConnectionResponse *Event_as_OnConnectionResponse() const {
    return Event_type() == Net::Events_OnConnectionResponse ? static_cast<const Net::OnConnectionResponse *>(Event()) : nullptr;
  }
  const Net::OnConnection *Event_as_OnConnection() const {
    return Event_type() == Net::Events_OnConnection ? static_cast<const Net::OnConnection *>(Event()) : nullptr;
  }
  const Net::OnMoveTo *Event_as_OnMoveTo() const {
    return Event_type() == Net::Events_OnMoveTo ? static_cast<const Net::OnMoveTo *>(Event()) : nullptr;
  }
  const Net::SyncTransform *Event_as_SyncTransform() const {
    return Event_type() == Net::Events_SyncTransform ? static_cast<const Net::SyncTransform *>(Event()) : nullptr;
  }
  const Net::OnPlayerJoined *Event_as_OnPlayerJoined() const {
    return Event_type() == Net::Events_OnPlayerJoined ? static_cast<const Net::OnPlayerJoined *>(Event()) : nullptr;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_EVENT_TYPE, 1) &&
           VerifyOffset(verifier, VT_EVENT) &&
           VerifyEvents(verifier, Event(), Event_type()) &&
           verifier.EndTable();
  }
};

template<> inline const Net::OnConnectionResponse *Header::Event_as<Net::OnConnectionResponse>() const {
  return Event_as_OnConnectionResponse();
}

template<> inline const Net::OnConnection *Header::Event_as<Net::OnConnection>() const {
  return Event_as_OnConnection();
}

template<> inline const Net::OnMoveTo *Header::Event_as<Net::OnMoveTo>() const {
  return Event_as_OnMoveTo();
}

template<> inline const Net::SyncTransform *Header::Event_as<Net::SyncTransform>() const {
  return Event_as_SyncTransform();
}

template<> inline const Net::OnPlayerJoined *Header::Event_as<Net::OnPlayerJoined>() const {
  return Event_as_OnPlayerJoined();
}

struct HeaderBuilder {
  typedef Header Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_Event_type(Net::Events Event_type) {
    fbb_.AddElement<uint8_t>(Header::VT_EVENT_TYPE, static_cast<uint8_t>(Event_type), 0);
  }
  void add_Event(::flatbuffers::Offset<void> Event) {
    fbb_.AddOffset(Header::VT_EVENT, Event);
  }
  explicit HeaderBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Header> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Header>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Header> CreateHeader(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    Net::Events Event_type = Net::Events_NONE,
    ::flatbuffers::Offset<void> Event = 0) {
  HeaderBuilder builder_(_fbb);
  builder_.add_Event(Event);
  builder_.add_Event_type(Event_type);
  return builder_.Finish();
}

inline bool VerifyEvents(::flatbuffers::Verifier &verifier, const void *obj, Events type) {
  switch (type) {
    case Events_NONE: {
      return true;
    }
    case Events_OnConnectionResponse: {
      auto ptr = reinterpret_cast<const Net::OnConnectionResponse *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Events_OnConnection: {
      auto ptr = reinterpret_cast<const Net::OnConnection *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Events_OnMoveTo: {
      auto ptr = reinterpret_cast<const Net::OnMoveTo *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Events_SyncTransform: {
      auto ptr = reinterpret_cast<const Net::SyncTransform *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Events_OnPlayerJoined: {
      auto ptr = reinterpret_cast<const Net::OnPlayerJoined *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyEventsVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (::flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyEvents(
        verifier,  values->Get(i), types->GetEnum<Events>(i))) {
      return false;
    }
  }
  return true;
}

inline const Net::Header *GetHeader(const void *buf) {
  return ::flatbuffers::GetRoot<Net::Header>(buf);
}

inline const Net::Header *GetSizePrefixedHeader(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<Net::Header>(buf);
}

inline bool VerifyHeaderBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Net::Header>(nullptr);
}

inline bool VerifySizePrefixedHeaderBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Net::Header>(nullptr);
}

inline void FinishHeaderBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<Net::Header> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedHeaderBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<Net::Header> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Net

#endif  // FLATBUFFERS_GENERATED_EVENTS_NET_H_
