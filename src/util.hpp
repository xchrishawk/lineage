/**
 * @file	util.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <algorithm>
#include <functional>

/* -- Types -- */

namespace lineage
{

  /**
   * Allows using enum classes as a key in `std::unordered_map`.
   */
  struct enum_class_hash
  {

    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }

  };

  /**
   * Class which performs a deferred action when it is destroyed.
   */
  class defer
  {
  public:

    /**
     * Constructs a new `lineage::defer` instance.
     *
     * @param action
     * The action to execute when this object is destroyed.
     */
    defer(std::function<void(void)> action)
      : m_action(action)
    { }

    ~defer()
    {
      m_action();
    }

  private:
    std::function<void(void)> m_action;
  };

}

/* -- Procedures -- */

namespace lineage
{

  /**
   * Returns the size of the specified array.
   */
  template <typename T, size_t size>
  constexpr size_t array_size(const T(&)[size])
  {
    return size;
  }

  /**
   * Clamps `value` to between `min` and `max`.
   */
  template <typename T>
  void clamp(T& value, T min, T max)
  {
    if (value < min)
      value = min;
    else if (value > max)
      value = max;
  }

  /**
   * Removes all instances of `value` from `container`.
   */
  template <typename TContainer, typename TValue>
  void remove_all(TContainer& container, const TValue& value)
  {
    container.erase(std::remove(container.begin(),
                                container.end(),
                                value),
                    container.end());
  }

}
