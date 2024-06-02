#pragma once
#include <memory>
#include <mutex>
#include <iostream>
/**�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
*��    �ߣ�lyf                                                �U
*ְ    ҵ��������Ѫ����ԯ                                              �U
*CSND��ַ���U
**GitHub ���U
*�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g
*����ʱ�䣺
*����������
*
*
*�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g
*����ʱ��:
*�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a
//                .-~~~~~~~~~-._       _.-~~~~~~~~~-.
//            __.'              ~.   .~              `.__
//          .'//              ����\./����               \\`.
//        .'//                     |                     \\`.
//      .'// .-~"""""""~~~~-._     |     _,-~~~~"""""""~-. \\`.
//    .'//.-"                 `-.  |  .-'                 "-.\\`.
//  .'//______.============-..   \ | /   ..-============.______\\`.
//.'______________________________\|/______________________________`.
*/
#include <memory>
#include<mutex>
#include<iostream>

template<typename T>
class Singleton {
protected:
    Singleton() = default;//����д�ɱ�������������ã������ⲿ���á�
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator=(const Singleton<T>&st) = delete;
    static std::shared_ptr<T> m_ptr;
public:
    static std::shared_ptr<T> Get_M_ptr() {
        std::once_flag flag;
        std::call_once(flag, []() {
            std::cout << "Get_M_ptr" << std::endl;
            m_ptr = std::shared_ptr<T>(new T);
            });
        return m_ptr;
    }
    void PrintAddress() {
        std::cout << m_ptr.get() << std::endl;
    }
    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
};
template<typename T>
std::shared_ptr<T> Singleton<T>::m_ptr = nullptr;