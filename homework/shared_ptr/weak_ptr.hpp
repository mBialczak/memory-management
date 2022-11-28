#pragma once

#include "shared_ptr.hpp"

#include <type_traits>

namespace my {
template <typename Type, typename ManagedType>
class shared_ptr;

template <typename Type, typename ManagedType = Type>
class weak_ptr {
public:
    template <typename OtherType, typename OtherManagedType>
    friend class shared_ptr;

    template <typename OtherType, typename OtherManagedType>
    friend class weak_ptr;

    ~weak_ptr();

    constexpr weak_ptr() noexcept;

    template <typename OtherType,
              typename OtherManagedType,
              typename = std::enable_if_t<std::is_convertible_v<OtherType*, Type*>>>
    weak_ptr(const shared_ptr<OtherType, OtherManagedType>& other) noexcept;

    weak_ptr(const weak_ptr& other) noexcept;

    template <class OtherType,
              typename = std::enable_if_t<std::is_convertible_v<OtherType*, Type*>>>
    weak_ptr(const weak_ptr<OtherType>& other) noexcept;

    weak_ptr(weak_ptr&& other) noexcept;

    template <class OtherType,
              typename = std::enable_if_t<std::is_convertible_v<OtherType*, Type*>>>
    weak_ptr(weak_ptr<OtherType>&& other) noexcept;

    weak_ptr& operator=(const weak_ptr& other) noexcept;

    template <class OtherType,
              typename = std::enable_if_t<std::is_convertible_v<OtherType&, Type&>>>
    weak_ptr& operator=(const weak_ptr<OtherType>& other) noexcept;

    template <class OtherType,
              typename OtherManagedType,
              typename = std::enable_if_t<std::is_convertible_v<OtherType&, Type&>>>
    weak_ptr& operator=(const shared_ptr<OtherType, OtherManagedType>& other) noexcept;

    weak_ptr& operator=(weak_ptr&& other) noexcept;

    template <class OtherType,
              typename = std::enable_if_t<std::is_convertible_v<OtherType&, Type&>>>
    weak_ptr& operator=(weak_ptr<OtherType>&& other) noexcept;

    bool expired() const noexcept;
    long use_count() const noexcept;
    shared_ptr<Type, ManagedType> lock();
    void reset() noexcept;

    //     // ================= OPTIONAL ========================
    //     // void swap( weak_ptr& r ) noexcept; NOTE: OPTIONAL

private:
    shared_ptr<Type, ManagedType>::ControlBlock* ctrlBlock_;
    Type* stored_ptr_;
};

template <typename Type, typename ManagedType>
constexpr weak_ptr<Type, ManagedType>::weak_ptr() noexcept
    // TODO: VERIFY
    //  : ctrlBlock_(nullptr),
    //    ptr_(nullptr) {
    : ctrlBlock_(nullptr),
      //   ptr_(nullptr) {
      stored_ptr_(nullptr) {
}

template <typename Type, typename ManagedType>
template <typename OtherType, typename OtherManagedType, typename>
weak_ptr<Type, ManagedType>::weak_ptr(const shared_ptr<OtherType, OtherManagedType>& other) noexcept
    // weak_ptr<Type, ManagedType>::weak_ptr(const shared_ptr<OtherType, ManagedType>& other) noexcept
    // TODO: VERIFY
    //  : ctrlBlock_(reinterpret_cast<shared_ptr<Type>::ControlBlock*>(other.ctrlBlock_)),
    //    ptr_(other.ptr_) {
    : ctrlBlock_(reinterpret_cast<shared_ptr<Type, ManagedType>::ControlBlock*>(other.ctrlBlock_)),
      stored_ptr_(other.stored_ptr_) {
    if (ctrlBlock_ && ctrlBlock_->ptr_) {
        // ptr_ = (other.ctrlBlock_->ptr_);
        ctrlBlock_->weakCount_++;
    }
}

template <typename Type, typename ManagedType>
weak_ptr<Type, ManagedType>::weak_ptr(const weak_ptr& other) noexcept
    : ctrlBlock_(other.ctrlBlock_),
      //   ptr_(other.ptr_) {
      stored_ptr_(other.stored_ptr_) {
    // TODO: VERIFY
    // : ctrlBlock_(other.ctrlBlock_),
    //   ptr_(other.ctrlBlock_->ptr_) {
    if (ctrlBlock_ && ctrlBlock_->ptr_) {
        ctrlBlock_->weakCount_++;
    }
}

template <typename Type, typename ManagedType>
template <class OtherType, typename>
weak_ptr<Type, ManagedType>::weak_ptr(const weak_ptr<OtherType>& other) noexcept
    : ctrlBlock_(reinterpret_cast<shared_ptr<Type, ManagedType>::ControlBlock*>(other.ctrlBlock_)),
      //   ptr_(other.ptr_) {
      stored_ptr_(other.stored_ptr_) {
    // TODO: VERIFY
    // : ctrlBlock_(reinterpret_cast<shared_ptr<Type>::ControlBlock*>(other.ctrlBlock_)),
    //   ptr_(other.ctrlBlock_->ptr_) {
    // if (ctrlBlock_) {
    if (ctrlBlock_ && ctrlBlock_->ptr_) {
        ctrlBlock_->weakCount_++;
    }
}

template <typename Type, typename ManagedType>
weak_ptr<Type, ManagedType>::weak_ptr(weak_ptr&& other) noexcept
    : ctrlBlock_(other.ctrlBlock_),
      //   ptr_(other.ptr_) {
      stored_ptr_(other.stored_ptr_) {
    other.stored_ptr_ = nullptr;
    other.ctrlBlock_ = nullptr;
}

template <typename Type, typename ManagedType>
template <class OtherType, typename>
weak_ptr<Type, ManagedType>::weak_ptr(weak_ptr<OtherType>&& other) noexcept
    : ctrlBlock_(reinterpret_cast<shared_ptr<Type, ManagedType>::ControlBlock*>(other.ctrlBlock_)),
      //   ptr_(other.ptr_) {
      stored_ptr_(other.stored_ptr_) {
    other.stored_ptr_ = nullptr;
    other.ctrlBlock_ = nullptr;
}

template <typename Type, typename ManagedType>
weak_ptr<Type, ManagedType>::~weak_ptr() {
    reset();
}

// TODO: VERIFY might need huge rethinks
template <typename Type, typename ManagedType>
void weak_ptr<Type, ManagedType>::reset() noexcept {
    if (ctrlBlock_) {
        ctrlBlock_->weakCount_--;
        if (ctrlBlock_->weakCount_ == 0
            // TODO: VERIFY
            && ctrlBlock_->sharedCount_ == 0) {
            delete ctrlBlock_;
        }
    }
    ctrlBlock_ = nullptr;
    stored_ptr_ = nullptr;
}

template <typename Type, typename ManagedType>
weak_ptr<Type, ManagedType>& weak_ptr<Type, ManagedType>::operator=(const weak_ptr& other) noexcept {
    ctrlBlock_ = other.ctrlBlock_;
    // ptr_ = other.ptr_;
    stored_ptr_ = other.stored_ptr_;
    if (ctrlBlock_ && ctrlBlock_->ptr_) {
        ctrlBlock_->weakCount_++;
    }
    return *this;
}

template <typename Type, typename ManagedType>
template <class OtherType, typename>
weak_ptr<Type, ManagedType>& weak_ptr<Type, ManagedType>::operator=(const weak_ptr<OtherType>& other) noexcept {
    ctrlBlock_ = reinterpret_cast<shared_ptr<Type, ManagedType>::ControlBlock*>(other.ctrlBlock_);
    stored_ptr_ = other.stored_ptr_;
    if (ctrlBlock_ && ctrlBlock_->ptr_) {
        ctrlBlock_->weakCount_++;
    }

    return *this;
}

template <typename Type, typename ManagedType>
template <class OtherType, typename OtherManagedType, typename>
weak_ptr<Type, ManagedType>&
weak_ptr<Type, ManagedType>::operator=(const shared_ptr<OtherType, OtherManagedType>& other) noexcept {
    // TODO: VERIFY
    //  ctrlBlock_ = reinterpret_cast<shared_ptr<Type>::ControlBlock*>(other.ctrlBlock_);
    //  ptr_ = other.ptr_;
    ctrlBlock_ = reinterpret_cast<shared_ptr<Type, ManagedType>::ControlBlock*>(other.ctrlBlock_);
    stored_ptr_ = other.stored_ptr_;
    // if (ctrlBlock_) {
    if (ctrlBlock_ && ctrlBlock_->ptr_) {
        // ptr_ = other.ctrlBlock_->ptr_;
        ctrlBlock_->weakCount_++;
    }

    return *this;
}

template <typename Type, typename ManagedType>
weak_ptr<Type, ManagedType>& weak_ptr<Type, ManagedType>::operator=(weak_ptr&& other) noexcept {
    ctrlBlock_ = other.ctrlBlock_;
    stored_ptr_ = other.stored_ptr_;
    other.ctrlBlock_ = nullptr;
    other.stored_ptr_ = nullptr;

    return *this;
}

template <typename Type, typename ManagedType>
template <class OtherType, typename>
weak_ptr<Type, ManagedType>& weak_ptr<Type, ManagedType>::operator=(weak_ptr<OtherType>&& other) noexcept {
    ctrlBlock_ = reinterpret_cast<shared_ptr<Type, ManagedType>::ControlBlock*>(other.ctrlBlock_);
    stored_ptr_ = other.stored_ptr_;
    other.ctrlBlock_ = nullptr;
    other.stored_ptr_ = nullptr;

    return *this;
}

template <typename Type, typename ManagedType>
long weak_ptr<Type, ManagedType>::use_count() const noexcept {
    return ctrlBlock_ ? ctrlBlock_->sharedCount_.load()
                      : 0;
}

template <typename Type, typename ManagedType>
bool weak_ptr<Type, ManagedType>::expired() const noexcept {
    return !static_cast<bool>(use_count());
}

template <typename Type, typename ManagedType>
shared_ptr<Type, ManagedType> weak_ptr<Type, ManagedType>::lock() {
    return expired() ? shared_ptr<Type, ManagedType>() : shared_ptr<Type, ManagedType>(*this);
}
// TODO: VERIFY
//  template <typename Type>
//  weak_ptr(shared_ptr<Type>) -> weak_ptr<Type>;
}  // namespace my
