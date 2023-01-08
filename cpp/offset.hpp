#ifndef EVL_UTILITY_OFFSET_HPP_
#define EVL_UTILITY_OFFSET_HPP_

#ifndef EVL_OFFSET_OF
#define EVL_OFFSET_OF(TYPE, field_name) ((std::size_t)(&(((const TYPE*)0)->field_name)))
#endif // EVL_OFFSET_OF

#endif // EVL_UTILITY_OFFSET_HPP_
