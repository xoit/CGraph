/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMessage.h
@Time: 2022/10/30 20:38
@Desc: 
***************************/

#ifndef CGRAPH_GMESSAGE_H
#define CGRAPH_GMESSAGE_H

#include <string>

#include "GMessageObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T, CUint capacity = CGRAPH_DEFAULT_RINGBUFFER_SIZE,
        std::enable_if_t<std::is_base_of<GMessageParam, T>::value, int> = 0>
class GMessage : public GMessageObject {
public:
    /**
     * 写入参数
     * @param value
     * @return
     */
    template<class TImpl,
            std::enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CVoid pub(const TImpl& value) {
        queue_.push(value);
    }

    /**
     * 获取参数
     * @param value
     * @return
     */
    template<class TImpl,
            std::enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CVoid sub(TImpl& value) {
        queue_.waitPop(value);
    }

    /**
     *
     * @param size
     * @return
     */
    CVoid setCapacity(CUint size) {
        queue_.setCapacity(size);
    }

    /**
     * 获取容量大小
     * @return
     */
    [[nodiscard]] CUint getCapacity() const {
        return queue_.getCapacity();
    }

    /**
     * 析构函数。释放前，要先释放队列中所有的信息
     */
    ~GMessage() override {
        queue_.clear();    // 将内部的队列释放
    }

private:
    UAtomicRingBufferQueue<T, capacity> queue_;
};


template<typename T, CUint capacity = CGRAPH_DEFAULT_RINGBUFFER_SIZE>
using GMessagePtr = GMessage<T, capacity> *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGE_H
