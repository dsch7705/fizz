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

template <PopupID>
struct Popup;

namespace detail {
template <typename T>
  requires(is_enum_type(^^T))
inline constexpr std::size_t enum_size_v = enumerators_of(^^T).size();

template <typename T>
  requires(is_enum_type(^^T))
constexpr std::string_view enum_str(T value)
{
  template for (constexpr auto e : std::define_static_array(enumerators_of(^^T)))
  {
    if ([:e:] == value) {
      return display_string_of(e);
    }
  }

  return "";
}

template <typename... Args>
  requires(is_convertible_type(^^Args, ^^std::string_view), ...)
consteval std::string_view join_str(Args... args)
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

consteval auto get_popup_types()
{
  std::vector<std::meta::info> popups;
  for (auto m : enumerators_of(^^PopupID)) {
    popups.push_back(substitute(^^Popup, {
                                             m}));
  }

  return popups;
}

}  // namespace detail

enum class PopupID { CreateBody, CreateConstraint };

inline constexpr std::string_view popupLabel(PopupID id)
{
  constexpr static std::array<const char*, detail::enum_size_v<PopupID>> labels = {"Create Body", "Create Constraint"};
  template for (constexpr auto i : std::views::iota(static_cast<std::size_t>(0), labels.size()))
  {
    constexpr auto msg =
        detail::join_str("No label has been defined for '", detail::enum_str(static_cast<PopupID>(i)), "'");
    static_assert(labels[i] != nullptr, msg);
  }

  return labels[static_cast<std::size_t>(id)];
}

template <PopupID id>
struct PopupView {};

template <PopupID id>
struct Popup {
  inline static bool shouldOpen;

  static void open() { shouldOpen = true; }

  static void render()
  {
    if (shouldOpen) {
      ImGui::OpenPopup(popupLabel(id).data());
      shouldOpen = false;
    }

    PopupView<id>::render();
  }
};

template <>
struct PopupView<PopupID::CreateBody> {
  static void render()
  {
    if (ImGui::BeginPopup(popupLabel(PopupID::CreateBody).data())) {
      ImGui::Button("Test");

      ImGui::EndPopup();
    }
  }
};

template <>
struct PopupView<PopupID::CreateConstraint> {
  static void render()
  {
    if (ImGui::BeginPopup(popupLabel(PopupID::CreateConstraint).data())) {
      ImGui::Button("Test");

      ImGui::EndPopup();
    }
  }
};

inline void render()
{
  static constexpr auto popups = std::define_static_array(detail::get_popup_types());
  template for (constexpr auto p : popups)
  {
    [:p:] ::render();
  }
}

};  // namespace Editor