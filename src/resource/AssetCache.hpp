#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <iostream>

class AssetCache
{
private:
   AssetCache() = default;
public:

    typedef std::shared_ptr<void> Value;

    static AssetCache& getInstance()
    {
        static AssetCache instance;
        return instance;
    }

    AssetCache(const AssetCache &other) = delete;
    AssetCache operator=(const AssetCache &) = delete;
    AssetCache(AssetCache &&other) = delete;
    AssetCache &operator=(AssetCache &&other) = delete;

    template <class T>
    void add(const std::string &id, std::shared_ptr<T> const &t);

    template <class T>
    bool remove(const std::string &id);

    template <class T>
    std::shared_ptr<T> get(const std::string &id);

    template <class T>
    std::vector<std::shared_ptr<T>> getAll();

    template <class T>
    bool contains(const std::string &id);

    void clear()
    {
        m_data.clear();
    }

private:
    struct Key
    {
        std::type_index TypeIndex;
        const std::string id;
    };

    struct KeyHash
    {
        std::size_t operator()(Key const &key) const
        {
            size_t res = 17;
            res = res * 31 + std::hash<std::type_index>()( key.TypeIndex );
            res = res * 31 + std::hash<std::string>()( key.id );
            return res;
        }
    };

    struct KeyEquality
    {
        bool operator()(Key const &a, Key const &b) const
        {
            return a.TypeIndex == b.TypeIndex && a.id == b.id;
        }
    };

    using DataT = std::unordered_map<Key, Value, KeyHash, KeyEquality>;
    DataT m_data;
};

template <class T>
void AssetCache::add(const std::string &id, std::shared_ptr<T> const &t)
{
    assert(t != nullptr);

    auto entry = m_data.emplace(Key{std::type_index(typeid(T)), id}, t);

    assert(entry.second);
}

template <class T>
bool AssetCache::remove(const std::string &id)
{
   size_t num_removed = m_data.erase(Key{std::type_index(typeid(T)), id});
   return num_removed > 0;
}

template <class T>
std::shared_ptr<T> AssetCache::get(const std::string &id)
{
    auto entry = m_data.find({std::type_index(typeid(T)), id});

    assert(entry != m_data.end());

    return std::static_pointer_cast<T>(entry->second);
}

template <class T>
std::vector<std::shared_ptr<T>> AssetCache::getAll()
{
    std::vector<std::shared_ptr<T>> vals;

    for(auto& [key, value] : m_data)
    {
        if (key.TypeIndex == std::type_index(typeid(T)))
        {
            vals.push_back(std::static_pointer_cast<T>(value));
        }
    }

    return vals;
}

template <class T>
bool AssetCache::contains(const std::string &id)
{
    auto entry = m_data.find({std::type_index(typeid(T)), id});

    return entry != m_data.end();
}