#define namespace ilrd
{

template <typename T> //must have lock() and unlock() functions
class ScopeLock : 
{//uncopyable class
public: 
    explicit ScopeLock(T &lock_); 
    ~ScopeLock(); //NOEXEPT

private: 
    T *m_lock; 
    bool m_is_aquired; //for double release
    
    ScopeLock &operator =(const ScopeLock &o_); //DO NOT IMPLIMENT
    ScopeLock(const ScopeLock &o_); //DO NOT IMPLIMENT
}

template <typename T>
ScopeLock<T>::ScopeLock(T &lock_): m_lock(lock_), m_is_aquired(m_lock_.lock())
{}

template <typename T>
ScopeLock<T>::ScopeLock()
{
    if(true == m_is_aquired)
    {
        m_lock.unlock(); 
    }
}

}//end namespace ilrd
