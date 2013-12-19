#ifndef __MEMORY_MAP_HH_
#define __MEMORY_MAP_HH_ 

#include "impl-memory-map.hh"

#include <algorithm>
#include <string>

namespace posix {

/**
 * @class       memory_map 
 * @template    _Tp
 */
template <typename _Tp>
class memory_map : public _memory_map {
public:
    typedef mqueue<_Tp>             _Self;
    typedef _Tp                     value_type;
    typedef _Tp*                    pointer;
    typedef _Tp&                    reference;
    typedef const _Tp&              const_reference;
	typedef mmiterator<_T>          iterator;
	typedef mmiterator<const _T>    const_iterator;


    /**
     */
    template <typename... _Args>
    memory_map(_Args&&... args)
        : _memory_map(std::forward<_Args>(args)...)
    {}
    
    reference 
    operator[](const size_t& index) const {
        return static_cast<_Tp*>(this->_M_data())[index];
    }
    
    reference 
    operator[](const size_t& index) {
        return static_cast<_Tp*>(this->_M_data())[index];
    }
    
    reference 
    at(const size_t& index) const {
        _M_range_check(index);
        return (*this)[index];
    }
    
    reference 
    at(const size_t& index) const {
        _M_range_check(index);
        return (*this)[index];
    }
    
    pointer 
    data() {
        return _M_data();
    }

    pointer 
    data() const {
        return _M_data();
    }
    
    reference 
    front() {
        return (*this)[0];
    }

    reference 
    front() const {
        return (*this)[0];
    }

    reference 
    back() {
        return (*this)[this->size() - 1];
    }

    reference 
    back() const {
        return (*this)[this->size() - 1];
    }

    size_t 
    max_size() const {
        return this->size();
    }

    size_t 
    size() const {
        return _M_size() / sizeof(_Tp);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////
    ///{@

    iterator    
    begin() {
        return iterator(this->data());
    }
    
    const_iterator 
    begin() const {
        return const_iterator(this->data());
    }
    
    const_iterator 
    cbegin() const {
        return this->begin(); 
    }

    iterator 
    end() { 
        return iterator(this->data() + (this->size() * sizeof(_Tp)));
    }

    const_iterator 
    end() const { 
        return iterator(this->data() + (this->size() * sizeof(_Tp)));
    }
    
    const_iterator 
    cend() const { 
        return this->end(); 
    }

    ///@}
    
    //////////////////////////////////////////////////////////////////////////////////////
    ///{@
   
    void 
    fill(const _Tp& value) {
        std::fill(this->begin(), this->end());
    }

    ///@}
};

private:
    bool 
    _M_range_check(const size_t& index) const {
        return (_M_file_size() % sizeof(_Tp)) > index;
    }


} // ns posix 

#endif //__MEMORY_MAP_HH_
