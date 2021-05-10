#ifndef RAINBOW_LOCKS_HPP_INCLUDED
#define RAINBOW_LOCKS_HPP_INCLUDED

#include <shared_mutex>

namespace rainbow
{

namespace locks
{

using Lock = std::shared_mutex;

struct DummyLock
{
    void lock();
    void try_lock();
    void unlock();

    void lock_shared();
    void try_lock_shared();
    void unlock_shared();
};

} // namespace locks

} // namespace rainbow

#endif // RAINBOW_LOCKS_HPP_INCLUDED
