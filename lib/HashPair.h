#ifndef HASHPAIR_H
#define HASHPAIR_H

#include <functional>

// Credit to YoungForest on Stack Overflow for this solution https://stackoverflow.com/a/62035742
namespace hash_tuple {
    template <typename TT> struct hash {
        size_t operator()(TT const &tt) const { return std::hash<TT>()(tt); }
    };

    // from boost (functional/hash):
    // see http://www.boost.org/doc/libs/1_35_0/doc/html/hash/combine.html template
    template <class T> inline void hash_combine(std::size_t &seed, T const &v) {
        seed ^= hash_tuple::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    // Recursive template code derived from Matthieu M.
    template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
    struct HashValueImpl {
        void operator()(size_t &seed, Tuple const &tuple) const {
            HashValueImpl<Tuple, Index - 1>{}(seed, tuple);
            hash_combine(seed, std::get<Index>(tuple));
        }
    };
    template <class Tuple> struct HashValueImpl<Tuple, 0> {
        void operator()(size_t &seed, Tuple const &tuple) const {
            hash_combine(seed, std::get<0>(tuple));
        }
    };

    template <typename... TT> struct hash<std::tuple<TT...>> {
        size_t operator()(std::tuple<TT...> const &tt) const {
            size_t seed = 0;
            HashValueImpl<std::tuple<TT...>>{}(seed, tt);
            return seed;
        }
    };
    // auxiliary generic functions to create a hash value using a seed
    template <typename T> inline void hash_val(std::size_t &seed, const T &val) {
        hash_combine(seed, val);
    }

    template <typename T, typename... Types>
    inline void hash_val(std::size_t &seed, const T &val, const Types &... args) {
        hash_combine(seed, val);
        hash_val(seed, args...);
    }

    template <typename... Types>
    inline std::size_t hash_val(const Types &... args) {
        std::size_t seed = 0;
        hash_val(seed, args...);
        return seed;
    }

    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &p) const {
            return hash_val(p.first, p.second);
        }
    };
}

#endif