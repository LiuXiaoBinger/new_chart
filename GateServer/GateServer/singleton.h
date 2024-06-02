#pragma once
#include <memory>
#include <mutex>
#include <iostream>
/**══════════════════════════════════╗
*作    者：lyf                                                ║
*职    业：我以我血荐轩辕                                              ║
*CSND地址：║
**GitHub ：║
*═══════════════════════════════════╣
*创建时间：
*功能描述：
*
*
*═══════════════════════════════════╣
*结束时间:
*═══════════════════════════════════╝
//                .-~~~~~~~~~-._       _.-~~~~~~~~~-.
//            __.'              ~.   .~              `.__
//          .'//              西南\./联大               \\`.
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
    Singleton() = default;//构造写成保护，让子类调用，避免外部调用。
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