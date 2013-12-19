
#ifndef __IMPL_MEMORY_MAP_HH_
#define __IMPL_MEMORY_MAP_HH_ 

#include <string>


namespace posix {

//////////////////////////////////////////////////////////////////////////////////////////
// MEMORY MAP ITErATOR 
//////////////////////////////////////////////////////////////////////////////////////////

template<class _T>
class mmiterator : public std::iterator<std::random_access_iterator_tag, _T> { 
public:
	typedef typename std::iterator<std::random_access_iterator_tag
		                         , _T>::value_type value_type;
	
	typedef typename std::iterator<std::random_access_iterator_tag
		                         , _T>::difference_type difference_type;
	
	typedef typename
	std::iterator<std::random_access_iterator_tag, _T>::pointer pointer;
	
	typedef typename
	std::iterator<std::random_access_iterator_tag, _T>::reference reference;

	
	mmiterator()
		:value(0x0)
	{}

	mmiterator(pointer value)
		:value(value)
	{ }

	template<typename _T2>
	mmiterator(const mmiterator<_T2> &rhs) 
		: value(&(*rhs))
	{ }

	template<typename _T2>
	mmiterator& operator=(const mmiterator<_T2> &rhs) {
		this->value = rhs.value;//&(*rhs);
		return *this;
	}
	
	mmiterator& operator++() { this->value++; return *this; }
	mmiterator& operator--() { this->value--; return *this; }

	mmiterator& operator++(int) { return mmiterator(this->value++); }
	mmiterator& operator--(int) { return mmiterator(this->value--); }
	
	mmiterator operator+(const difference_type &d) const {
		return mmiterator(this->value + d);
	}
	mmiterator operator-(const difference_type &d) const {
		return mmiterator(pointer(this->value - d));
	}
	
	mmiterator operator+=(const difference_type &d) {
		this->value += d;
                return *this;
	}
	mmiterator& operator-=(const difference_type &d) {
		this->value += d;
                return *this;
	}

	reference operator*() const {
		return *(this->value);
	}

	pointer operator->() const {
		return this->value;
	}

	reference operator[](const difference_type &d) const {
		return this->value[d];
	}
	
	template<typename _Tp>
	friend bool operator==(const mmiterator<_Tp>&, const mmiterator<_Tp>&);

	template<typename _Tp>
	friend bool operator!=(const mmiterator<_Tp>&, const mmiterator<_Tp>&);
	
	template<typename _Tp>
	friend bool operator>=(const mmiterator<_Tp>&, const mmiterator<_Tp>&);

	template<typename _Tp>
	friend bool operator<=(const mmiterator<_Tp>&, const mmiterator<_Tp>&);

	template<typename _Tp>
	friend bool operator>(const mmiterator<_Tp>&, const mmiterator<_Tp>&);

	template<typename _Tp>
	friend bool operator<(const mmiterator<_Tp>&, const mmiterator<_Tp>&);
	
	
	template<typename _Tp>
	friend typename mmiterator<_Tp>::difference_type operator+(
			const mmiterator<_Tp>&,
			const mmiterator<_Tp>&
			);
	
	template<typename _Tp>
	friend typename mmiterator<_Tp>::difference_type operator-(
			const mmiterator<_Tp>&,
			const mmiterator<_Tp>&
			);

private:
	_T* value;
};


template<typename T>
bool operator==(const mmiterator<T> &r1, const mmiterator<T> &r2) {
	return r1.value == r2.value;
}

template<typename T>
bool operator!=(const mmiterator<T> &r1, const mmiterator<T> &r2) {
	return r1.value != r2.value;
}

template<typename T>
bool operator>=(const mmiterator<T> &r1, const mmiterator<T> &r2) {
	return r1.value >= r2.value;
}

template<typename T>
bool operator<=(const mmiterator<T> &r1, const mmiterator<T> &r2) {
	return r1.value <= r2.value;
}

template<typename T>
bool operator>(const mmiterator<T> &r1, const mmiterator<T> &r2) {
	return r1.value > r2.value;
}

template<typename T>
bool operator<(const mmiterator<T> &r1, const mmiterator<T> &r2) {
	return r1.value < r2.value;
}

template<typename T>
typename mmiterator<T>::difference_type operator+(const mmiterator<T> &r1,
                                                  const mmiterator<T> &r2) {
	return mmiterator<T>(r1.value + r2.value);
}

template<typename T>
typename mmiterator<T>::difference_type operator-(const mmiterator<T> &r1,
                                                  const mmiterator<T> &r2) {
	return r1.value - r2.value;
}


//////////////////////////////////////////////////////////////////////////////////////////
// MEMORY MAP IMPLEMENTATION CLASS
//////////////////////////////////////////////////////////////////////////////////////////

class _memory_map {
public:
    _memory_map(const std::string& file_path);
    virtual ~_memory_map();
    
    operator bool() const { return _M_ptr != 0x0; }

    size_t open(int permission, int scope, int offset = 0);
    void close();

protected:
    void* _M_data() { return _M_ptr; }
    size_t _M_size() const { return _M_file_size; }
    bool _M_truncated(const size_t& block_size) const;

private:
    const std::string _M_file_path;
    const int _M_offset;
    size_t _M_file_size;
    int _M_permission;
    int _M_scope;

    int _M_fd;
    void* _M_ptr;
};

} // ns posix 

#endif //__IMPL_MEMORY_MAP_HH_
