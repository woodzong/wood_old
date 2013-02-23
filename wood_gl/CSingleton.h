#ifndef SINGLETON_T_H
#define SINGLETON_T_H


class CDummyLock
{
};

template <class TYPE, class LOCK = CDummyLock>
class CSingletonT
{
public:
    static TYPE* Instance();

private:
	CSingletonT(){};
    //CSingletonT(const CSingletonT&);
	~CSingletonT(){}; 

private:
	TYPE s_aGuestInstance;
	static CSingletonT<TYPE, LOCK> m_aHostInstance;
};

/////////////////////////////////////////
template <class TYPE, class LOCK>
CSingletonT<TYPE, LOCK> CSingletonT<TYPE, LOCK>::m_aHostInstance;

template <class TYPE, class LOCK>
TYPE* 
CSingletonT<TYPE, LOCK>::Instance()
{
    //Lock Guard
   return &(m_aHostInstance.s_aGuestInstance);
}

#endif