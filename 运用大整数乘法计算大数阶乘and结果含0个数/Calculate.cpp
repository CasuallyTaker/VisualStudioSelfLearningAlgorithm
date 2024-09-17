#include <iostream>

#include <algorithm>

#include <cmath>

#include <cstring>

using namespace std;



// 最大长度N，分块长度K

//const int N = 10001;
//
//const int K = 10000;
const int N = 10001;

const int K = 10000;

const int T = (int)log10(K); // K=10000, T=4



void char_to_int(char* s, int* c)

{

    // *s = 1234567890

    // *c = 0000 .... 0012 3456 7890 (K=10000, T=4, c[0] c[1] .... c[N-2] c[N-1])

    // i  = 0         N-3  N-2  N-1  (c的下标索引i)

    int L = (int)strlen(s);

    for (int i = N - 1; i >= 0; i--) {

        c[i] = 0;

        if (L >= 0) {

            for (int j = 0; j < T; j++) {

                if (L - T + j < 0) {

                    continue;

                }

                c[i] = c[i] * 10 + s[L - T + j] - '0';

            }

            L -= T;

        }

    }

}



void output(int* c)

{

    // *c = 0000 .... 0012 3456 7890 (K=10000, T=4, c[0] c[1] .... c[N-2] c[N-1])

    // i  = 0         N-3  N-2  N-1  (c的下标索引i)

    // out= 1234567890

    bool flag = true;

    for (int i = 0; i < N; i++) {

        if (flag && c[i] == 0) {

            continue;

        }

        if (flag) { // 第一个非0数字：大数的头块

            //printf("%d", c[i]);
            cout << c[i];

            flag = false;

        }

        else { // 大数头块之后的 T 数字块

            int TK = K / 10;

            int tmp = c[i];

            while (TK) {

                //printf("%d", tmp / TK);
                cout << tmp / TK;

                tmp %= TK;

                TK /= 10;

            }

        }

    }

    //printf("\n");
    cout << endl;

}



void add(int* a, int* b, int* c)

{

    int carry = 0;

    for (int i = N - 1; i >= 0; i--) {

        c[i] = 0;

        c[i] = a[i] + b[i] + carry;

        carry = c[i] / K;

        c[i] = c[i] % K;

    }



}



void sub(int* a, int* b, int* c)

{

    int borrow = 0;

    for (int i = N - 1; i >= 0; i--) {

        c[i] = 0;

        c[i] = a[i] - b[i] - borrow;

        if (c[i] >= 0) {

            borrow = 0;

        }

        else {

            c[i] = c[i] + K;

            borrow = 1;

        }

    }



}



void mul1(int* a, int b, int* c)

{

    long long tmp = 0;

    int carry = 0;

    for (int i = N - 1; i >= 0; i--) {

        tmp = (long long)a[i] * (long long)b + (long long)carry;

        c[i] = (int)(tmp % K);

        carry = (int)(tmp / K);

    }



}



void mul2(int* a, int* b, int* c)

{

    long long tmp = 0;

    int carry = 0;

    for (int i = N - 1; i >= 0; i--) {

        c[i] = 0;

    }

    for (int i = N - 1; i >= 0; i--) { // b[i]

        carry = 0;

        for (int j = N - 1, idc = i; j >= 0 && idc >= 0; j--, idc--) { // a[j]

            tmp = (long long)a[j] * (long long)b[i] + (long long)carry + (long long)c[idc];

            c[idc] = (int)(tmp % K);

            carry = (int)(tmp / K);

        }

    }



}



int div(int* a, int b, int* c)

{

    long long tmp = 0;

    int remain = 0;

    for (int i = 0; i < N; i++) {

        tmp = (long long)a[i] + (long long)remain * (long long)K;

        c[i] = (int)(tmp / b);

        remain = (int)(tmp % b);

    }

    return remain;



}



int calc(int n)
{
    /********* Begin *********/
    if (n == 0) return 0; //0的阶乘为1，故0的阶乘中0的个数为0,直接返回即可。

    int a[N] = { 0 };    //初始化大整数阶乘数组
    a[N - 1] = 1;    //最高位初始为1

    for (int i = 2; i <= n; ++i) {
        mul1(a, i, a);    //使用mul1函数循环计算阶乘
    }

    cout << "阶乘的计算结果：";
    output(a);



    int zero_count = 0; // 初始化0的计数器
    bool flag = false;   //表示还未遍历到第一个有效数位（阶乘结果为0000 0000 0000 ... n!）

    for (int i = 0; i < N; ++i) {

        if (a[i] == 0 && (!flag)) continue; // 如果当前块为0且未到第一个有效位，则跳过，继续检索下一块
        flag = true;        //已到有效数字位
        static int firstIndex = i;  //只记录第一个有效块的索引

        /*cout << "第一个有效块索引为" << firstIndex << ",";
        cout<< "此时是n的阶乘数组的第" << i << "有效块，数值为：" << a[i] << endl;*/

        int number = a[i];

        //int block_Unzero_count = 0;   //块内非前置0（前置0数即，0xxx,00xx,000x,0000）的个数，即x的个数

        //第一个有效块无需计算前置0
        if (firstIndex == i) {
            while (number > 0) {
                if (number % 10 == 0) zero_count++;
                number /= 10;
            }
            continue;  //第一个有效块计算完毕，直接跳到下一个有效块
        }

        //其余有效块0的计算
        //for循环更好，正好是四位，同时解决前置0数的统计问题
        for (int j = 0; j < T; j++) {
            if (number % 10 == 0) zero_count++;
            number /= 10;
        }

    }

    return zero_count;
    /********* End *********/
}



int main(int argc, const char* argv[]) {



    int n;



    //scanf("%d", &n);
    cin >> n;



    int tot = calc(n);

    //printf("%d\n", tot);

    cout << "0的个数为："<<tot << endl;

    //cout << tot << endl;



    return 0;

}


