#ifndef TYPES_H
#define TYPES_H
#ifdef __cplusplus
extern "C" {
#endif

typedef char            int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef long            int32;
typedef unsigned short  uint16;
typedef unsigned long   uint32;
typedef float           float32;
typedef long double     float64;

typedef unsigned char   BOOL;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef NEGATIVE
#define NEGATIVE 0
#endif

#ifndef POSITIVE
#define POSITIVE 1
#endif

#define U8Bit(n)    ((uint8)(1 << n))
#define U16Bit(n)   ((uint16)(1 << n))
#define U32Bit(n)   ((uint32)(1 << n))

union Data32 {
    uint32 word;
    uint16 half_word[2];
    uint8 byte[4];
};

union Data16 {
    uint16 half_word;
    uint8 byte[2];
};

#define Rad2Degree(rad) ((rad) * 57.295779513082320876798154814105f)
#define Degree2Rad(deg) ((deg) * .01745329251994329576923690768489f)

/*
* OffsetOf - 求取一个结构体某个成员的偏移量
*
* @type: 结构体类型
* @member: 成员名称
*/
#define OffsetOf(type, member) ((unsigned long) &((type *)0)->member)
/*
* ContainerOf - 根据成员地址@ptr和结构体类型，求取所在结构体的地址
*
* @ptr: 结构体成员指针
* @type: 结构体类型
* @member: 成员名称
*/
#define ContainerOf(ptr, type, member) ((type *)((uint8 *)(ptr) - OffsetOf(type, member)))


#ifdef __cplusplus
}
#endif
#endif
