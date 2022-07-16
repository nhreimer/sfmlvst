#ifndef RJRZMBK_NONCOPYABLE_HPP
#define RJRZMBK_NONCOPYABLE_HPP

namespace rj
{
  struct Noncopyable
  {
    Noncopyable() = default;

    Noncopyable( const Noncopyable& ) = delete;
    Noncopyable& operator=( const Noncopyable& ) = delete;
  };
}

#endif//RJRZMBK_NONCOPYABLE_HPP
