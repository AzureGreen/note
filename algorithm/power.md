# 快速幂算法

## 思路
将指数y转成二进制格式，二进制每一位对应的幂值是上一位对应幂值的平方，然后通过判断y当前位是否为1，决定是否乘上当前位的幂值。

```c++
uint64_t Pow(int x, int y) {
    uint64_t ret = 1;
    uint64_t power_2_n = x;

    do {
        if (y & 1) {
            ret = ret * power_2_n;
        }
        power_2_n =  power_2_n * power_2_n;
    } while (y >>= 1);
    return ret;
}
```