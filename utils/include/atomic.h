/**
 *  \file   atomic.h
 *
 *  \brief
 *  \author  lfl 
 *  \addtogroup atomic
 *  原子操作
 *  
 *  @{ 
 *   
 */


#ifndef    __ATOMIC__H__
#define    __ATOMIC__H__

#include "cpu.h"

typedef  void  * volatile atomic ;



/**
 * @brief 原子写
 * @param [in] atom 原子变量指针
 * @param [in] val  要写入的值 
 * @return           
 * @date    2013/8/11
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
static inline void atomicWrite(atomic *atom, int val){
    CPUirqd();
    *(int *)atom = val;
    CPUirqe();
}

/**
 * @brief 原子写
 * @param [in] atom 原子变量指针 
 * @return           
 * @date    2013/8/11
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
static inline int atomicRead(atomic *atom){
   return (int)*atom;
}


/**
 * @brief 原子置1
 * @param [in] atom 原子变量指针 
 * @return           
 * @date    2013/8/11
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
static inline void atomicSet(atomic *atom){
    atomicWrite(atom, 1);
}

/**
 * @brief 原子清0
 * @param [in] atom 原子变量指针 
 * @return           
 * @date    2013/8/11
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
static inline void atomicClear(atomic *atom){
    atomicWrite(atom, 0);
}

/**
 * @brief 原子加
 * @param [in] atom 原子变量指针 
 * @param [in] val   
 * @return           
 * @date    2013/8/11
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
static inline void atomicAdd(atomic *atom,int val){
    CPUirqd();
    *(int *)(atom) += val;
    CPUirqe();
}

/**
 * @brief 原子加1
 * @param [in] atom 原子变量指针 
 * @return           
 * @date    2013/8/11
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
static inline void atomicInc(atomic *atom){
   atomicAdd(atom,1);
}

/**
 * @brief 原子减1
 * @param [in] atom 原子变量指针 
 * @return           
 * @date    2013/8/11
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
static inline void atomicDec(atomic *atom){
   atomicAdd(atom,-1);
}

/**
 * @brief 原子测试
 * @param [in] atom 原子变量指针 
 * @return  原子变量为1 返回1，为0返回0         
 * @date    2013/8/11
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
static inline unsigned int  atomicTest(atomic *atom){  
    return !! (int)(*atom) ;
}


/**
 * @brief 原子测试并清0
 * @param [in] atom 原子变量指针 
 * @return  原子变量为1 返回1，为0返回0         
 * @date    2013/8/11
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
static inline unsigned int atomicTestClear(atomic *atom){
   if (*atom) {
      atomicWrite(atom, 0);
      return 1;
   }else
      return 0;
}


#endif
//! @}
 
