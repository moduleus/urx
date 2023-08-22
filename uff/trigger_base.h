#pragma once

#include <optional>

#include <uff/trigger_in.h>
#include <uff/trigger_out.h>

namespace uff {

class TriggerBase {
 public:
  // CTOR & DTOR
  TriggerBase() = default;
  TriggerBase(const TriggerBase&) = default;
  TriggerBase(TriggerBase&&) = default;
  TriggerBase(std::optional<TriggerIn> trigger_in, std::optional<TriggerOut> trigger_out)
      : _trigger_in(trigger_in), _trigger_out(std::move(trigger_out)){};
  virtual ~TriggerBase() = default;

  // Operators
  TriggerBase& operator=(const TriggerBase& other) noexcept = default;
  TriggerBase& operator=(TriggerBase&& other) noexcept = default;
  bool operator==(const TriggerBase& other) const {
    return (_trigger_in == other._trigger_in && _trigger_out == other._trigger_out);
  }
  inline bool operator!=(const TriggerBase& other) const { return !(*this == other); }

  // Accessors
  // Trigger in
  inline std::optional<TriggerIn> triggerIn() const { return _trigger_in; }
  inline void setTriggerIn(const std::optional<TriggerIn>& trigger_in) { _trigger_in = trigger_in; }

  // Trigger out
  inline std::optional<TriggerOut> triggerOut() const { return _trigger_out; }
  inline void setTriggerOut(const std::optional<TriggerOut>& trigger_out) {
    _trigger_out = trigger_out;
  }

  // Members
 protected:
  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> _trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> _trigger_out = std::nullopt;
};

}  // namespace uff
