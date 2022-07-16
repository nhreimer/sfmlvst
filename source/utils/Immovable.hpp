#ifndef RJRZMBK_IMMOVABLE_HPP
#define RJRZMBK_IMMOVABLE_HPP

namespace rj
{
  struct Immovable
  {
    Immovable() = default;

    Immovable( Immovable&& ) = delete;
    Immovable&& operator=( const Immovable& ) = delete;
  };
}

#endif//RJRZMBK_IMMOVABLE_HPP
