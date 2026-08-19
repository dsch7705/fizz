#pragma once

#include <algorithm>
#include <array>
#include <format>
#include <meta>
#include <print>
#include <ranges>
#include <string_view>
#include <unordered_map>

#include "imgui.h"

namespace Editor {

// Forward declarations
enum class PopupID;
enum class WindowID;

template <typename T>
concept IDType = is_same_type(^^T, ^^PopupID) || is_same_type(^^T, ^^WindowID);

template <IDType auto id>
struct Widget;

template <PopupID>
struct Popup;

template <WindowID>
struct Window;

namespace detail {
template <typename T>
  requires(is_enum_type(^^T))
inline constexpr std::size_t enum_size_v = enumerators_of(^^T).size();

template <typename T>
  requires(is_enum_type(^^T))
constexpr std::string_view enum_str(T value)
{
  return display_string_of(std::meta::reflect_constant(value));
}

template <IDType T>
constexpr std::string_view id_str(T id)
{
  std::string_view ds = display_string_of(std::meta::reflect_constant(id));

  if (std::size_t last_semi = ds.find_last_of(':'); last_semi != ds.npos) {
    ds = ds.substr(last_semi + 1);
  }

  auto is_upper = [](char c) -> bool { return (c >= 'A' && c <= 'Z'); };

  std::vector<char> str;
  for (auto [first, last_c] = std::pair{true, ' '}; char c : ds) {
    if (first) {
      first = false;
    }
    else if (is_upper(c) && !is_upper(last_c)) {
      str.push_back(' ');
    }

    str.push_back(c);
    last_c = c;
  }
  str.push_back('\0');

  return std::define_static_string(std::string_view{str.data()});
}

template <typename... Args>
  requires(is_convertible_type(^^Args, ^^std::string_view), ...)
constexpr std::string_view join_str(Args... args)
{
  std::vector<char> data;
  template for (auto r : {args...})
  {
    std::string_view sv{r};
    for (auto c : sv) {
      data.push_back(c);
    }
  }
  data.push_back('\0');

  return std::define_static_string(std::string_view{data.data()});
}

// clang-format off
consteval auto get_popup_types()
{
  std::vector<std::meta::info> popups;
  for (auto m : enumerators_of(^^PopupID)) {
    popups.push_back(substitute(^^Popup, {m}));
  }

  return std::define_static_array(popups);
}

consteval auto get_window_types()
{
  std::vector<std::meta::info> windows;
  for (auto m : enumerators_of(^^WindowID)) {
    windows.push_back(substitute(^^Window, {m}));
  }

  return std::define_static_array(windows);
}
// clang-format on

}  // namespace detail

enum class PopupID {
  CreateBody,
  CreateConstraint,
};
enum class WindowID {
  Inspector,
};

template <IDType auto id>
struct Widget {};

template <PopupID id>
struct Popup {
  inline static bool shouldOpen;

  static void open() { shouldOpen = true; }

  static void render()
  {
    static constexpr const char* id_str = detail::id_str(id).data();

    if (ImGui::BeginPopup(id_str)) {
      Widget<id>::render();
      ImGui::EndPopup();
    }

    if (shouldOpen) {
      ImGui::OpenPopup(id_str);
      shouldOpen = false;
    }
  }
};

template <WindowID id>
struct Window {
  inline static bool isOpen;

  static void open() { isOpen = true; }
  static void close() { isOpen = false; }

  static void render()
  {
    static constexpr const char* id_str = detail::id_str(id).data();

    if (isOpen) {
      if (ImGui::Begin(id_str, &isOpen)) {
        Widget<id>::render();
      }
      ImGui::End();
    }
  }
};

// Window Widgets
template <>
struct Widget<WindowID::Inspector> {
  static void render()
  {
    ImGui::Text("Testing");
    //
  }
};

// Popup Widgets
template <>
struct Widget<PopupID::CreateBody> {
  static void render()
  {
    if (ImGui::Button("Close")) {
      ImGui::CloseCurrentPopup();
    }
  }
};

template <>
struct Widget<PopupID::CreateConstraint> {
  static void render() { ImGui::Button("Test"); }
};

inline void render()
{
  static constexpr auto windows = detail::get_window_types();
  template for (constexpr auto w : windows)
  {
    [:w:] ::render();
  }

  static constexpr auto popups = detail::get_popup_types();
  template for (constexpr auto p : popups)
  {
    [:p:] ::render();
  }
}

};  // namespace Editor