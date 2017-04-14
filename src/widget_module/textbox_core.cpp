#include <widget_module/widgets/textbox_core.hpp>
#include <painter_module/painter.hpp>
#include <system_module/events/paint_event.hpp>
#include <widget_module/coordinate.hpp>

#include <cstddef>

namespace twf {

Textbox_core::Textbox_core(const Glyph_string& string) : contents_{string} {};

void Textbox_core::scroll_up(std::size_t n) { // work here, top line is not showing
    for (std::size_t i = upper_bound_; i < contents_.size(); ++i) {
        if (i - upper_bound_ == this->geometry().width() - 1 ||
            contents_.at(i).str() == "\n") {
            upper_bound_ = i + 1;
            break;
        }
    }
    // set lower bound first??
    if (cursor_index_ > lower_bound_) {
        this->set_cursor_index(lower_bound_);
    }
}

void Textbox_core::scroll_down(std::size_t n) {
    if (upper_bound_ == 0) {
        return;
    }
    for (std::size_t i = upper_bound_ - 1; i != 0; --i) {
        if (contents_.at(i).str() == "\n" ||
            upper_bound_ - i == this->geometry().width() - 1) {
            upper_bound_ = i;
            break;
        }
    }
    if (cursor_index_ < upper_bound_) {
        this->set_cursor_index(upper_bound_);
    }
}

// Moves the cursor up n positions, scrolls if need be.
void Textbox_core::cursor_up(std::size_t n) {
    auto pos = this->position_from_index(cursor_index_);
    --pos.y;
    this->set_cursor_index(this->index_from_position(pos));
}

// Moves the cursor down n positions, scrolls if need be.
void Textbox_core::cursor_down(std::size_t n) {
    auto pos = this->position_from_index(cursor_index_);
    ++pos.y;
    this->set_cursor_index(this->index_from_position(pos));
}

// Moves the cursor left n positions, wraps if need be.
void Textbox_core::cursor_left(std::size_t n) {
    if (cursor_index_ != 0) {
        auto pos = this->position_from_index(cursor_index_);
        --pos.x;
        this->set_cursor_index(this->index_from_position(pos));
    }
}

// Moves the cursor right n positions, wraps if need be.
void Textbox_core::cursor_right(std::size_t n) {
    if (cursor_index_ != contents_.size()) {
        auto pos = this->position_from_index(cursor_index_);
        ++pos.x;
        this->set_cursor_index(this->index_from_position(pos));
    }
}

// Called by update, defines what it means to paint a widget that is a
// textbox_core.
bool Textbox_core::paint_event(const Paint_event& event) {
    Painter p{this};
    // Paint the visible sub-string.
    Glyph_string sub_str(std::begin(contents_) + upper_bound_,
                         std::end(contents_) + lower_bound_);
    p.put_at(0, 0, sub_str, false);
    // Move the cursor to the appropriate position.
    auto pos = this->position_from_index(cursor_index_);
    p.move(pos.x, pos.y);
    return Widget::paint_event(event);
}

// Finds the location in the Glyph_string where the given position refers to.
std::size_t Textbox_core::index_from_position(std::size_t x, std::size_t y) {
    Coordinate running_position;
    for (std::size_t i{upper_bound_}; i < contents_.size(); ++i) {
        // Exact match.
        if (running_position.x == x && running_position.y == y) {
            return i;
        }
        // Position is past last character on line.
        if (running_position.y == y && contents_.at(i).str() == "\n") {
            return i;
        }
        // Increment
        if (contents_.at(i).str() == "\n" ||
            running_position.x + 1 == this->geometry().width()) {
            ++running_position.y;
            running_position.x = 0;
        } else {
            ++running_position.x;
        }
    }
    return contents_.size();
}

std::size_t Textbox_core::index_from_position(Coordinate pos) {
    return this->index_from_position(pos.x, pos.y);
}

// Calculates the position of the cursor on the 2D screen from an index that
// cooresponds to a position within the Glyph_string contents_ member.
Coordinate Textbox_core::position_from_index(std::size_t index) {
    Coordinate position;
    for (std::size_t i{upper_bound_}; i < index; ++i) {
        if (contents_.at(i).str() == "\n" ||
            position.x + 1 == this->geometry().width()) {
            ++position.y;
            position.x = 0;
        } else {
            ++position.x;
        }
    }
    return position;
}

// Sets the position of the cursor in the Glyph_string. Possibly scrolls so
// that the index position is visible in the window.
void Textbox_core::set_cursor_index(std::size_t index) {
    cursor_index_ = index; // this was set to i, which is an enum, subtle bug...
    if (cursor_index_ < upper_bound_) {
        // scroll up as many times as needed
    } else if (cursor_index_ > lower_bound_) {
        // scroll down as many times as needed
    }
    auto pos = this->position_from_index(cursor_index_);
    Painter p{this};
    p.move(pos.x, pos.y);
}

}  // namespace twf
