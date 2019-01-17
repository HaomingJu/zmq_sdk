//
// Copyright 2018 Horizon Robotics.
//

#ifndef HOBOT_DMS_UTILS_MEMPOOL_MEMPOOL_H_
#define HOBOT_DMS_UTILS_MEMPOOL_MEMPOOL_H_

#include <hobot/hobot.h>
#include <stddef.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <utility>
#include <vector>


namespace Modo {

template <typename T>
class MemPool {
 public:
  template <typename... Args>
  static T *GetEx(Args &&... args) {
    MemPool<T> *pool = GetInstance();
    if (pool) {
      std::lock_guard<std::mutex> lck(pool->pool_mutex_);
      if (pool->free_list_.empty()) {
        if (pool->max_alloc_ != 0 && pool->max_alloc_ == pool->allocated_) {
          return NULL;
        } else if (pool->max_alloc_ > pool->allocated_ ||
                   pool->max_alloc_ == 0) {
          pool->AllocItem(std::forward<Args>(args)...);
        }
      }

      T *item = pool->free_list_.front();
      pool->free_list_.pop_front();
      pool->used_list_.push_back(item);

      return item;
    } else {
      return NULL;
    }
  }
  static T *Get() {
    MemPool<T> *pool = GetInstance();
    if (pool) {
      std::lock_guard<std::mutex> lck(pool->pool_mutex_);

      if (pool->free_list_.empty()) {
        return NULL;
      }

      T *item = pool->free_list_.front();
      pool->free_list_.pop_front();
      pool->used_list_.push_back(item);

      return item;
    } else {
      return NULL;
    }
  }
  template <typename... Args>
  static std::shared_ptr<T> GetSharedPtrEx(bool auto_release, Args &&... args) {
    T *item = NULL;
    item = GetEx(std::forward<Args>(args)...);
    if (item) {
      if (auto_release) {
        std::shared_ptr<T> sp_item(item, ItemDeleter);
        return sp_item;
      } else {
        std::shared_ptr<T> sp_item(item, IntemDeleterNull);
        return sp_item;
      }
    } else {
      return NULL;
    }
  }
  static std::shared_ptr<T> GetSharedPtr(bool auto_release = true) {
    T *item = Get();
    if (item) {
      if (auto_release) {
        std::shared_ptr<T> sp_item(item, ItemDeleter);
        return sp_item;
      } else {
        std::shared_ptr<T> sp_item(item, IntemDeleterNull);
        return sp_item;
      }
    } else {
      return NULL;
    }
  }
  static int Release(T *item) {
    MemPool<T> *pool = GetInstance();
    if (pool) {
      ItemDeleter(item);
    }
    return 0;
  }

  template <typename... Args>
  static bool Create(int pre_alloc, int max_alloc, Args &&... args) {
    std::lock_guard<std::mutex> lck(singleton_mutex_);

    if (pool_ptr_.get() == NULL) {
      pool_ptr_.reset(new MemPool<T>);
      pool_ptr_.get()->Init(pre_alloc, max_alloc, std::forward<Args>(args)...);
    } else {
      // LOGW_T(MODULE_TAG) << "mempool already initialized";
      return false;
    }
    return true;
  }

  static MemPool<T> *GetInstance() {
    std::lock_guard<std::mutex> lck(singleton_mutex_);

    if (pool_ptr_.get() == NULL) {
      //  LOGE_T(MODULE_TAG) << "please initialize MemPool<" << typeid(T).name()
      //                     << "> first";
      return NULL;
    }
    return pool_ptr_.get();
  }
  int GetAllocatedCnt() { return allocated_; }
  int GetMaxAllocCnt() { return max_alloc_; }
  int GetUsedCnt() {
    std::lock_guard<std::mutex> lck(pool_mutex_);
    return used_list_.size();
  }
  int GetFreeCnt() {
    std::lock_guard<std::mutex> lck(pool_mutex_);
    return free_list_.size();
  }
  static hobot::Module *repeater_;

 private:
  MemPool() {}
  ~MemPool();
  MemPool(const MemPool &other);
  MemPool &operator=(const MemPool &other);

  std::vector<T *> items_;
  std::list<T *> free_list_;
  std::list<T *> used_list_;
  int max_alloc_;
  int allocated_;
  std::mutex pool_mutex_;
  static std::mutex singleton_mutex_;
  static std::auto_ptr<MemPool<T> > pool_ptr_;
  friend class std::auto_ptr<MemPool<T> >;

  /* the invoker must hold pool_mutex before call AllocItem*/
  template <typename... Args>
  int AllocItem(Args &&... args) {
    T *item = new T(std::forward<Args>(args)...);
    items_.push_back(item);
    free_list_.push_front(item);
    allocated_++;
    return 0;
  }

  static void ItemDeleter(T *item) {
    if (!item) {
      return;
    }
    MemPool<T> *pool = GetInstance();
    if (pool) {
      std::lock_guard<std::mutex> lck(pool->pool_mutex_);
      auto iter = find(pool->used_list_.begin(), pool->used_list_.end(), item);
      if (iter == pool->used_list_.end()) {
        return;
      }

      item->Reset();

      pool->free_list_.push_back(item);
      pool->used_list_.erase(iter);
    }
    return;
  }

  static void IntemDeleterNull(T *item) { return; }

  template <typename... Args>
  int Init(int pre_alloc, int max_alloc, Args &&... args);
};

template <typename T>
std::mutex MemPool<T>::singleton_mutex_;
template <typename T>
std::auto_ptr<MemPool<T> > MemPool<T>::pool_ptr_;
template <typename T>
hobot::Module *MemPool<T>::repeater_;

template <typename T>
template <typename... Args>
int MemPool<T>::Init(int pre_alloc, int max_alloc, Args &&... args) {
  std::lock_guard<std::mutex> lck(pool_mutex_);
  allocated_ = 0;
  for (int i = 0; i < pre_alloc; i++) {
    AllocItem(std::forward<Args>(args)...);
  }
  used_list_.clear();
  max_alloc_ = max_alloc;
  repeater_ = new hobot::InputModule();
  return 0;
}

template <typename T>
MemPool<T>::~MemPool() {
  std::lock_guard<std::mutex> lck(pool_mutex_);
  for (size_t i = 0; i < items_.size(); i++) {
    delete items_[i];
  }
  items_.clear();
  free_list_.clear();
  used_list_.clear();

  delete repeater_;
}

template <typename T>
class MemPoolEx {
 public:
  template <typename... Args>
  T *GetEx(Args &&... args) {
    std::lock_guard<std::mutex> lck(pool_mutex_);
    if (free_list_.empty()) {
      if (max_alloc_ != 0 && max_alloc_ == allocated_) {
        return NULL;
      } else if (max_alloc_ > allocated_ || max_alloc_ == 0) {
        AllocItem(std::forward<Args>(args)...);
      }
    }
    if (free_list_.empty()) {
      return NULL;
    }
    T *item = free_list_.front();
    free_list_.pop_front();
    used_list_.push_back(item);

    return item;
  }
  T *Get() {
    std::lock_guard<std::mutex> lck(pool_mutex_);
    if (free_list_.empty()) {
      return NULL;
    }

    T *item = free_list_.front();
    free_list_.pop_front();
    used_list_.push_back(item);

    return item;
  }
  template <typename... Args>
  std::shared_ptr<T> GetSharedPtrEx(bool auto_release, Args &&... args) {
    T *item = NULL;
    item = GetEx(std::forward<Args>(args)...);
    if (item) {
      if (auto_release) {
        ItemDeleter deleter(static_pool_map_[this]);
        std::shared_ptr<T> sp_item(item, deleter);
        return sp_item;
      } else {
        std::shared_ptr<T> sp_item(item, ItemDeleterNull);
        return sp_item;
      }
    } else {
      return NULL;
    }
  }
  std::shared_ptr<T> GetSharedPtr(bool auto_release = true) {
    T *item = Get();
    if (item) {
      if (auto_release) {
        ItemDeleter deleter(static_pool_map_[this]);
        std::shared_ptr<T> sp_item(item, deleter);
        return sp_item;
      } else {
        std::shared_ptr<T> sp_item(item, ItemDeleterNull);
        return sp_item;
      }
    } else {
      return NULL;
    }
  }
  int Release(T *item) {
    ItemDeleter deleter(static_pool_map_[this]);
    deleter(item);
    return 0;
  }

  template <typename... Args>
  static std::shared_ptr<MemPoolEx<T> > Create(int pre_alloc, int max_alloc,
                                               Args &&... args) {
    MemPoolEx<T> *pool = new MemPoolEx<T>();
    pool->Init(pre_alloc, max_alloc, std::forward<Args>(args)...);

    auto deleter = [](MemPoolEx<T> *p) { delete p; };
    std::shared_ptr<MemPoolEx<T> > sp_pool(pool, deleter);

    {
      std::lock_guard<std::mutex> lck(static_pool_mutex_);

      // remove none referenced pools
      auto iter = static_pool_map_.begin();
      while (iter != static_pool_map_.end()) {
        if (iter->second.use_count() == 1) {
          iter = static_pool_map_.erase(iter);
        } else {
          ++iter;
        }
      }

      // cache new allocated pool's shared_ptr
      static_pool_map_[pool] = sp_pool;
    }
    return sp_pool;
  }

  int GetAllocatedCnt() { return allocated_; }
  int GetMaxAllocCnt() { return max_alloc_; }
  int GetUsedCnt() {
    std::lock_guard<std::mutex> lck(pool_mutex_);
    return used_list_.size();
  }
  int GetFreeCnt() {
    std::lock_guard<std::mutex> lck(pool_mutex_);
    return free_list_.size();
  }

  struct ItemDeleter {  // a verbose array deleter:
    explicit ItemDeleter(std::shared_ptr<MemPoolEx<T> > pool) : pool_(pool) {}
    std::shared_ptr<MemPoolEx<T> > pool_;

    void operator()(T *item) {
      if (!item) {
        return;
      }
      std::lock_guard<std::mutex> lck(pool_->pool_mutex_);
      auto iter =
          std::find(pool_->used_list_.begin(), pool_->used_list_.end(), item);
      if (iter == pool_->used_list_.end()) {
        return;
      }

      item->Reset();

      pool_->free_list_.push_back(item);
      pool_->used_list_.erase(iter);
      return;
    }
  };

 private:
  MemPoolEx() {}
  ~MemPoolEx();
  MemPoolEx(const MemPoolEx &other);
  MemPoolEx &operator=(const MemPoolEx &other);

  typedef std::unordered_map<MemPoolEx<T> *, std::shared_ptr<MemPoolEx<T> > >
      PoolMap;
  static std::mutex static_pool_mutex_;
  static PoolMap static_pool_map_;

  std::vector<T *> items_;
  std::list<T *> free_list_;
  std::list<T *> used_list_;
  int max_alloc_;
  int allocated_;
  std::mutex pool_mutex_;
  friend class std::auto_ptr<MemPoolEx<T> >;

  /* the invoker must hold pool_mutex before call AllocItem*/
  template <typename... Args>
  int AllocItem(Args &&... args) {
    T *item = new T(std::forward<Args>(args)...);
    items_.push_back(item);
    free_list_.push_front(item);
    allocated_++;
    return 0;
  }

  static void ItemDeleterNull(T *item) { return; }

  template <typename... Args>
  int Init(int pre_alloc, int max_alloc, Args &&... args);
};

template <typename T>
std::mutex MemPoolEx<T>::static_pool_mutex_;

template <typename T>
std::unordered_map<MemPoolEx<T> *, std::shared_ptr<MemPoolEx<T> > >
    MemPoolEx<T>::static_pool_map_;


template <typename T>
template <typename... Args>
int MemPoolEx<T>::Init(int pre_alloc, int max_alloc, Args &&... args) {
  std::lock_guard<std::mutex> lck(pool_mutex_);
  allocated_ = 0;
  for (int i = 0; i < pre_alloc; i++) {
    AllocItem(std::forward<Args>(args)...);
  }
  used_list_.clear();
  max_alloc_ = max_alloc;
  return 0;
}

template <typename T>
MemPoolEx<T>::~MemPoolEx() {
  std::lock_guard<std::mutex> lck(pool_mutex_);
  for (size_t i = 0; i < items_.size(); i++) {
    delete items_[i];
  }
  items_.clear();
  free_list_.clear();
  used_list_.clear();
}

class BaseMsg : public hobot::Message {
 public:
  int64_t ts_;

 protected:
  BaseMsg() {}
  virtual ~BaseMsg() {}
  virtual void Reset() = 0;
  virtual bool DeepCopy(const BaseMsg *) = 0;

 private:
  BaseMsg &operator=(const BaseMsg &other);
  BaseMsg(const BaseMsg &other);
};
typedef std::shared_ptr<BaseMsg> spBaseMsg;
}  // end of namespace HobotDMS

#endif  // HOBOT_DMS_UTILS_MEMPOOL_MEMPOOL_H_
