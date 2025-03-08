#include "util.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 条件错误检查函数
 *
 * 如果条件为真，则使用 perror 打印错误信息并退出程序
 *
 * @param condition 检查的条件
 * @param errmsg 错误信息字符串
 */
void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}