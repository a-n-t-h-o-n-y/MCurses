#ifndef CPPURSES_SYSTEM_EVENTS_ENABLE_EVENT_HPP
#define CPPURSES_SYSTEM_EVENTS_ENABLE_EVENT_HPP
#include <cppurses/system/event.hpp>

namespace cppurses {
class Event_handler;

/// Sent to a Widget that has been enabled.
/** Enabled Widget receive Events and are painted to the screen. */
class Enable_event : public Event {
   public:
    explicit Enable_event(Event_handler* receiver);
    bool send() const override;
    bool filter_send(Event_handler* filter) const override;
};

}  // namespace cppurses
#endif  // CPPURSES_SYSTEM_EVENTS_ENABLE_EVENT_HPP
