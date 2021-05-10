#ifndef RAINBOW_SHAREDPTR_HPP_INCLUDED
#define RAINBOW_SHAREDPTR_HPP_INCLUDED

#include <rainbow/locks.hpp>
#include <rainbow/memory/allocator.hpp>
#include <rainbow/object.hpp>

namespace rainbow
{

template<typename T, typename Lock = rainbow::locks::Lock>
class SharedPtr
{
public:
    SharedPtr() : SharedPtr(nullptr) {}

    SharedPtr(std::nullptr_t) : _controlBlock{nullptr}, _allocator{nullptr} {}

    template<typename... Args>
    SharedPtr(rainbow::memory::Allocator* allocator, Args&&... args)
        : _controlBlock{rainbow::new_<ControlBlock>(
              *allocator, std::forward<Args>(args)...)},
          _allocator{allocator}
    {
    }

    SharedPtr(const SharedPtr& other)
    {
        std::shared_lock lock{other._lock};

        _allocator    = other._allocator;
        _controlBlock = other._controlBlock;

        if(_controlBlock)
        {
            _controlBlock->owners++;
        }
    }

    SharedPtr(SharedPtr&& other)
    {
        std::unique_lock lock{other._lock};

        _allocator    = other._allocator;
        _controlBlock = other._controlBlock;

        other._controlBlock = nullptr;
        other._allocator    = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other)
    {
        release();

        {
            std::shared_lock lock{other._lock};

            _controlBlock = other._controlBlock;
            _allocator    = other._allocator;
        }

        if(_controlBlock)
        {
            _controlBlock->owners++;
        }
    }

    SharedPtr& operator=(SharedPtr&& other)
    {
        release();
        _controlBlock       = other._controlBlock;
        _allocator          = other._allocator;
        other._controlBlock = nullptr;
    }

    ~SharedPtr()
    {
        release();
    }

    T* get() const
    {
        if(_controlBlock)
        {
            return &_controlBlock->object;
        }
        else
        {
            return nullptr;
        }
    }

    T* operator->() const
    {
        return get();
    }

    explicit operator bool() const
    {
        return not _controlBlock.isNull();
    }

private:
    struct ControlBlock
    {
        T           object;
        std::size_t owners;

        template<typename... Args>
        ControlBlock(Args&&... args)
            : object{std::forward<Args>(args)...}, owners{1}
        {
        }
    };

    rainbow::Object<ControlBlock> _controlBlock;
    rainbow::memory::Allocator*   _allocator;
    mutable Lock                  _lock;

    void release()
    {
        if(_controlBlock and --_controlBlock->owners == 0)
        {
            rainbow::delete_(*_allocator, _controlBlock);
            _controlBlock = nullptr;
        }
    }
};

template<typename T, typename... Args>
SharedPtr<T> makeShared(rainbow::memory::Allocator& allocator, Args&&... args)
{
    return {&allocator, std::forward<Args>(args)...};
}

} // namespace rainbow

#endif // RAINBOW_SHAREDPTR_HPP_INCLUDED
