//
// Created by cc on 2020/1/9.
//

#ifndef QTALK_V2_FREQUENCYQUEUE_H
#define QTALK_V2_FREQUENCYQUEUE_H

#include <list>
#include <mutex>
#include <vector>

template <class T>
class FrequencyQueue {
public:
    FrequencyQueue(unsigned int maxSize) {
        _maxListSize = maxSize;
    }

    ~FrequencyQueue()
    {
        for(const auto* pItem : _list)
            delete pItem;
        _list.clear();
    }

public:
    void insert(T t)
    {
        std::unique_lock<std::mutex> lock(_mutex);

        auto it = std::find(_list.begin(), _list.end(), t);
        if(it != _list.end())
            _list.erase(it);
        _list.push_back(t);

        while (_list.size() > _maxListSize)
        {
            auto* old_item = _list.begin();
            _list.erase(old_item);
        }
    }

    bool contains(T t)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return std::find(_list.begin(), _list.end(), t) != _list.end();
    }

    std::vector<T> get(unsigned int count)
    {
        std::vector<T> res;
        auto it = _list.end();
        int index = 0;
        while (it > _list.begin() && index++ < count )
        {
            it--;
            res.push_back(*it);
        }

        return res;
    }

private:
    std::list<T>  _list; //
    unsigned int   _maxListSize{};
    std::mutex     _mutex;
};

#endif //QTALK_V2_FREQUENCYQUEUE_H
