/**
 * @file	util.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/12
 */

#pragma once

/* -- Includes -- */

#include <algorithm>

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

}

/* -- Procedures -- */

namespace lineage
{

  /**
   * Clamps `value` to between `min` and `max`.
   */
  template <typename T>
  T clamp(T value, T min, T max)
  {
    if (value < min)
      return min;
    else if (value > max)
      return max;
    else
      return value;
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