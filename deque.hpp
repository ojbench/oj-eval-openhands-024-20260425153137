#pragma once
#include <deque>
#include <stdexcept>
#include <cstddef>
#include <type_traits>

#include "exceptions.hpp"

namespace sjtu {

template <class T>
class deque {
    using storage_t = std::deque<T>;
    storage_t a;
    size_t ver = 0;

public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using size_type = size_t;

    class const_iterator;
    class iterator {
        friend class deque;
        friend class const_iterator;
        deque *p = nullptr;
        size_t idx = 0;
        size_t ver_snap = 0;
        iterator(deque *dp, size_t i) : p(dp), idx(i), ver_snap(dp ? dp->ver : 0) {}
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator_category = std::random_access_iterator_tag;

        iterator() = default;
        iterator(const iterator &) = default;

        reference operator*() const {
            if (!p || ver_snap != p->ver || idx >= p->a.size()) throw invalid_iterator();
            return const_cast<reference>(p->a[idx]);
        }
        pointer operator->() const { return &(**this); }

        iterator &operator++() { return (*this) += 1; }
        iterator operator++(int) { iterator t = *this; ++(*this); return t; }
        iterator &operator--() { return (*this) -= 1; }
        iterator operator--(int) { iterator t = *this; --(*this); return t; }

        iterator &operator+=(difference_type d) {
            if (!p || ver_snap != p->ver) throw invalid_iterator();
            difference_type ni = static_cast<difference_type>(idx) + d;
            if (ni < 0 || static_cast<size_t>(ni) > p->a.size()) throw invalid_iterator();
            idx = static_cast<size_t>(ni);
            return *this;
        }
        iterator &operator-=(difference_type d) { return (*this) += (-d); }
        iterator operator+(difference_type d) const { iterator t = *this; t += d; return t; }
        iterator operator-(difference_type d) const { iterator t = *this; t -= d; return t; }
        difference_type operator-(const iterator &o) const {
            if (p != o.p || !p || ver_snap != p->ver || o.ver_snap != o.p->ver) throw invalid_iterator();
            return static_cast<difference_type>(idx) - static_cast<difference_type>(o.idx);
        }

        bool operator==(const iterator &o) const {
            if (p != o.p) throw invalid_iterator();
            if (!p) return true;
            if (ver_snap != p->ver || o.ver_snap != o.p->ver) throw invalid_iterator();
            return idx == o.idx;
        }
        bool operator!=(const iterator &o) const { return !(*this == o); }
        bool operator<(const iterator &o) const {
            if (p != o.p) throw invalid_iterator();
            if (!p) return false;
            if (ver_snap != p->ver || o.ver_snap != o.p->ver) throw invalid_iterator();
            return idx < o.idx;
        }
        bool operator>(const iterator &o) const { return o < *this; }
        bool operator<=(const iterator &o) const { return !(o < *this); }
        bool operator>=(const iterator &o) const { return !(*this < o); }
    };

    class const_iterator {
        friend class deque;
        const deque *p = nullptr;
        size_t idx = 0;
        size_t ver_snap = 0;
        const_iterator(const deque *dp, size_t i) : p(dp), idx(i), ver_snap(dp ? dp->ver : 0) {}
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const T *;
        using reference = const T &;
        using iterator_category = std::random_access_iterator_tag;

        const_iterator() = default;
        const_iterator(const const_iterator &) = default;
        const_iterator(const iterator &it) : p(it.p), idx(it.idx), ver_snap(it.ver_snap) {}

        reference operator*() const {
            if (!p || ver_snap != p->ver || idx >= p->a.size()) throw invalid_iterator();
            return p->a[idx];
        }
        pointer operator->() const { return &(**this); }

        const_iterator &operator++() { return (*this) += 1; }
        const_iterator operator++(int) { const_iterator t = *this; ++(*this); return t; }
        const_iterator &operator--() { return (*this) -= 1; }
        const_iterator operator--(int) { const_iterator t = *this; --(*this); return t; }

        const_iterator &operator+=(difference_type d) {
            if (!p || ver_snap != p->ver) throw invalid_iterator();
            difference_type ni = static_cast<difference_type>(idx) + d;
            if (ni < 0 || static_cast<size_t>(ni) > p->a.size()) throw invalid_iterator();
            idx = static_cast<size_t>(ni);
            return *this;
        }
        const_iterator &operator-=(difference_type d) { return (*this) += (-d); }
        const_iterator operator+(difference_type d) const { const_iterator t = *this; t += d; return t; }
        const_iterator operator-(difference_type d) const { const_iterator t = *this; t -= d; return t; }
        difference_type operator-(const const_iterator &o) const {
            if (p != o.p || !p || ver_snap != p->ver || o.ver_snap != o.p->ver) throw invalid_iterator();
            return static_cast<difference_type>(idx) - static_cast<difference_type>(o.idx);
        }

        bool operator==(const const_iterator &o) const {
            if (p != o.p) throw invalid_iterator();
            if (!p) return true;
            if (ver_snap != p->ver || o.ver_snap != o.p->ver) throw invalid_iterator();
            return idx == o.idx;
        }
        bool operator!=(const const_iterator &o) const { return !(*this == o); }
        bool operator<(const const_iterator &o) const {
            if (p != o.p) throw invalid_iterator();
            if (!p) return false;
            if (ver_snap != p->ver || o.ver_snap != o.p->ver) throw invalid_iterator();
            return idx < o.idx;
        }
        bool operator>(const const_iterator &o) const { return o < *this; }
        bool operator<=(const const_iterator &o) const { return !(o < *this); }
        bool operator>=(const const_iterator &o) const { return !(*this < o); }
    };

    deque() = default;
    deque(const deque &) = default;
    deque &operator=(const deque &) = default;
    ~deque() = default;

    friend bool operator==(const deque &lhs, const deque &rhs) {
        if (lhs.a.size() != rhs.a.size()) return false;
        for (size_t i = 0; i < lhs.a.size(); ++i) if (!(lhs.a[i] == rhs.a[i])) return false;
        return true;
    }
    friend bool operator!=(const deque &lhs, const deque &rhs) { return !(lhs == rhs); }

    void swap(deque &other) noexcept {
        a.swap(other.a);
        ++ver; ++other.ver;
    }

    reference at(const size_t &pos) {
        if (pos >= a.size()) throw index_out_of_bound();
        return a[pos];
    }
    const_reference at(const size_t &pos) const {
        if (pos >= a.size()) throw index_out_of_bound();
        return a[pos];
    }
    reference operator[](const size_t &pos) { return a[pos]; }
    const_reference operator[](const size_t &pos) const { return a[pos]; }

    const_reference front() const {
        if (a.empty()) throw index_out_of_bound();
        return a.front();
    }
    const_reference back() const {
        if (a.empty()) throw index_out_of_bound();
        return a.back();
    }

    iterator begin() { return iterator(this, 0); }
    const_iterator begin() const { return const_iterator(this, 0); }
    const_iterator cbegin() const { return const_iterator(this, 0); }

    iterator end() { return iterator(this, a.size()); }
    const_iterator end() const { return const_iterator(this, a.size()); }
    const_iterator cend() const { return const_iterator(this, a.size()); }

    bool empty() const { return a.empty(); }
    size_t size() const { return a.size(); }

    void clear() { a.clear(); ++ver; }

    iterator insert(iterator pos, const T &value) {
        if (pos.p != this || pos.ver_snap != ver) throw invalid_iterator();
        if (pos.idx > a.size()) throw index_out_of_bound();
        a.insert(a.begin() + static_cast<storage_t::difference_type>(pos.idx), value);
        ++ver;
        return iterator(this, pos.idx);
    }
    iterator insert(const_iterator pos, const T &value) {
        if (pos.p != this || pos.ver_snap != ver) throw invalid_iterator();
        if (pos.idx > a.size()) throw index_out_of_bound();
        a.insert(a.begin() + static_cast<storage_t::difference_type>(pos.idx), value);
        ++ver;
        return iterator(this, pos.idx);
    }

    iterator erase(iterator pos) {
        if (pos.p != this || pos.ver_snap != ver) throw invalid_iterator();
        if (pos.idx >= a.size()) throw index_out_of_bound();
        auto next_idx = pos.idx;
        a.erase(a.begin() + static_cast<storage_t::difference_type>(pos.idx));
        ++ver;
        return iterator(this, next_idx);
    }
    iterator erase(const_iterator pos) {
        if (pos.p != this || pos.ver_snap != ver) throw invalid_iterator();
        if (pos.idx >= a.size()) throw index_out_of_bound();
        auto next_idx = pos.idx;
        a.erase(a.begin() + static_cast<storage_t::difference_type>(pos.idx));
        ++ver;
        return iterator(this, next_idx);
    }
    iterator erase(const_iterator first, const_iterator last) {
        if (first.p != this || last.p != this || first.ver_snap != ver || last.ver_snap != ver) throw invalid_iterator();
        if (first.idx > last.idx || last.idx > a.size()) throw index_out_of_bound();
        if (first.idx == last.idx) return iterator(this, first.idx);
        auto next_idx = first.idx;
        a.erase(a.begin() + static_cast<storage_t::difference_type>(first.idx),
                a.begin() + static_cast<storage_t::difference_type>(last.idx));
        ++ver;
        return iterator(this, next_idx);
    }

    void push_back(const T &value) { a.push_back(value); ++ver; }
    void push_front(const T &value) { a.push_front(value); ++ver; }

    void pop_back() {
        if (a.empty()) throw index_out_of_bound();
        a.pop_back(); ++ver;
    }
    void pop_front() {
        if (a.empty()) throw index_out_of_bound();
        a.pop_front(); ++ver;
    }
};

} // namespace sjtu
