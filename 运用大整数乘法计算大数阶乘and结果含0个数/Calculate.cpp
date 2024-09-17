#include <iostream>

#include <algorithm>

#include <cmath>

#include <cstring>

using namespace std;



// ��󳤶�N���ֿ鳤��K

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

    // i  = 0         N-3  N-2  N-1  (c���±�����i)

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

    // i  = 0         N-3  N-2  N-1  (c���±�����i)

    // out= 1234567890

    bool flag = true;

    for (int i = 0; i < N; i++) {

        if (flag && c[i] == 0) {

            continue;

        }

        if (flag) { // ��һ����0���֣�������ͷ��

            //printf("%d", c[i]);
            cout << c[i];

            flag = false;

        }

        else { // ����ͷ��֮��� T ���ֿ�

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
    if (n == 0) return 0; //0�Ľ׳�Ϊ1����0�Ľ׳���0�ĸ���Ϊ0,ֱ�ӷ��ؼ��ɡ�

    int a[N] = { 0 };    //��ʼ���������׳�����
    a[N - 1] = 1;    //���λ��ʼΪ1

    for (int i = 2; i <= n; ++i) {
        mul1(a, i, a);    //ʹ��mul1����ѭ������׳�
    }

    cout << "�׳˵ļ�������";
    output(a);



    int zero_count = 0; // ��ʼ��0�ļ�����
    bool flag = false;   //��ʾ��δ��������һ����Ч��λ���׳˽��Ϊ0000 0000 0000 ... n!��

    for (int i = 0; i < N; ++i) {

        if (a[i] == 0 && (!flag)) continue; // �����ǰ��Ϊ0��δ����һ����Чλ��������������������һ��
        flag = true;        //�ѵ���Ч����λ
        static int firstIndex = i;  //ֻ��¼��һ����Ч�������

        /*cout << "��һ����Ч������Ϊ" << firstIndex << ",";
        cout<< "��ʱ��n�Ľ׳�����ĵ�" << i << "��Ч�飬��ֵΪ��" << a[i] << endl;*/

        int number = a[i];

        //int block_Unzero_count = 0;   //���ڷ�ǰ��0��ǰ��0������0xxx,00xx,000x,0000���ĸ�������x�ĸ���

        //��һ����Ч���������ǰ��0
        if (firstIndex == i) {
            while (number > 0) {
                if (number % 10 == 0) zero_count++;
                number /= 10;
            }
            continue;  //��һ����Ч�������ϣ�ֱ��������һ����Ч��
        }

        //������Ч��0�ļ���
        //forѭ�����ã���������λ��ͬʱ���ǰ��0����ͳ������
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

    cout << "0�ĸ���Ϊ��"<<tot << endl;

    //cout << tot << endl;



    return 0;

}


