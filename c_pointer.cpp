#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

// tested x86, c++17


void TestPtrBasic()
{
    // 주소 + 정수 => 주소
    // 배열은 포인터로 완전 분해됨
    // arr[0] == *arr == *(arr + 0)
    // arr[1] == *(arr + 1)
    // arr[2] == *(arr + 2)

    //int* arrp1;         // 1차원 배열 포인터
    //int (*arrp2)[3];    // 2차원 배열 포인터
    //int (*arrp3)[2][3]; // 3차원 배열 포인터

    int arr[2][3] = { {1, 2, 3}, {4, 5, 6} };
    int (*arrp)[3] = arr;

    // arr == arr[0] == &arr[0][0], 주소는 같지만 대상체의 크기가 다름
    printf("ptr: %p, size: %d\n", arr, sizeof(arr));     // 24
    printf("ptr: %p, size: %d\n", arrp, sizeof(arrp));   // 4
    printf("ptr: %p, size: %d\n", *arrp, sizeof(*arrp)); // 12

    printf("ptr: %p, size: %d\n", arrp + 0, sizeof(arrp + 0));  // 4
    printf("ptr: %p, size: %d\n", arrp + 1, sizeof(arrp + 1));  // 4

    printf("ptr: %p, size: %d\n", *(arrp + 0), sizeof(*(arrp + 0)));  // 12
    printf("ptr: %p, size: %d\n", *(arrp + 1), sizeof(*(arrp + 1)));  // 12
}


void Arg2DArray(int (*arrp)[3])
{
    // arrp       // arr
    // arrp + 1   // arr + 1
    // *arrp       // arr[0]
    // *(arrp + 1) // arr[1]

    // **arrp          // arr[0][0]
    // *(*arrp + 1)    // arr[0][1]
    // *(*arrp + 2)    // arr[0][2]
    // *(*(arrp + 1))  // arr[1][0]
    // *(*(arrp + 1) + 1)  // arr[1][1]
    // *(*(arrp + 1) + 2)  // arr[1][2]

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++) {
            printf("arr[%d][%d] %d\n", i, j, *(*(arrp + i) + j));
            printf("arr[%d][%d] %d\n", i, j, *(arrp[i] + j));
            printf("arr[%d][%d] %d\n", i, j, arrp[i][j]);
        }
}

void TestArg2DArray()
{
    //void Arg2DArray(int (*)[3]);
    int arr[2][3] = { {1, 2, 3}, {4, 5, 6} };
    int(*arrp)[3];
    arrp = arr;

    Arg2DArray(arrp);
}


void TestStrPtr()
{
    const char* str = "StrPointer1";
    char str2[] = "StrPointer2";
    str2[0] = 'P';

    printf("str: %s, size: %d\n", str, sizeof(str));
    printf("str: %s, size: %d\n", str2, sizeof(str2));

    const char* arrp[3];
    const char** arrpp;

    arrp[0] = "one";
    arrp[1] = "two";
    arrp[2] = "three";
    arrpp = arrp;

    printf("str: %s, %s, %s\n", *arrpp, *(arrpp + 1), *(arrpp + 2));
    printf("arrp size: %d, arrpp size: %d", sizeof(arrp), sizeof(arrpp));

    const char* const arrp2[3] = { "four", "five", "six" };

    printf("str: %s, %s, %s\n", arrp2[0], arrp2[1], arrp2[2]);

    // char* str3;
    // strcpy(str3, "archie"); // error

    char str3[10];
    strcpy_s(str3, sizeof(str3), "strCpy");
    printf("str: %s\n", str3);

    char* str4 = (char*)malloc(sizeof(char) * 10);
    strcpy_s(str4, sizeof(char) * 10, "strCpy");
    printf("str: %s\n", str4);
    free(str4);
}


int VarAdd(int count, ...)
{
    // typedef char* va_list;
    // void va_start(va_list list, last);
    // type va_arg(va_list, type);
    // void va_end(va_list list);

    int sum = 0;

    va_list list;
    va_start(list, count);

    for (int i = 0; i < count; i++)
        sum += va_arg(list, int);

    va_end(list);
    return sum;
}

void TestVarAdd()
{
    // int VarAdd(int, ...);
    int sum = VarAdd(3, 10, 100, 1000);
    printf("10 + 100 + 1000 = %d", sum);
}


void TestVoidPtr()
{
    // 어떠한 형 변환(캐스트 연산자 사용) 없이도 void형 포인터 변수에 대입 가능
    // void형 포인터 변수에서 값을 읽을 때는 반드시 캐스트 연산자 사용
    // * 연산자(간접지정 연산자)를 사용할 때는 항상 캐스트 연산자 사용

    int num1 = 10;
    float num2 = 20.f;
    void* voidp;

    voidp = &num1;
    printf("%d\n", *(int*)voidp);

    voidp = &num2;
    printf("%.2f\n", *(float*)voidp);

    char str[] = "voidptr";
    voidp = str;
    printf("str: %s\n", (char*)voidp);
    printf("(char*)voidp: %c %c %c\n",
        *(char*)voidp,
        *((char*)voidp + 1),
        *((char*)voidp + 2));

    // C 언어 표준에서 자료형 변환은 l - value를 생성하지 않는다고 규정
    // // (char*)voidp++ l-value error
    // for (; *(char*)voidp; (char*)voidp++)
    //    printf("%c\n", *(char*)voidp);

    // void 포인터를 다른 자료형의 포인터에 할당한 뒤 ++, -- 연산자 사용
    char* charp = (char*)voidp;
    for (; *charp; charp++)
        printf("%c\n", *charp);
}


void ArgTwoPtr(const char** arr)
{
    *arr++ = "four"; // *arr++ == *(arr++)
    *arr++ = "five";
    *arr = "six";
}

void TestArgTwoPtr()
{
    const char* str[3] = { "one", "two", "three" };
    printf("%s %s %s\n", str[0], str[1], str[2]);
 
    ArgTwoPtr(str);
    printf("%s %s %s\n", str[0], str[1], str[2]);
}


int FuncAdd(int num)
{
    return num + num;
}

int FuncMul(int num)
{
    return num * num;
}

void TestFuncPtr()
{
    int (*func[2])(int) = { FuncAdd, FuncMul };
    // func[0] = FuncAdd;
    // func[1] = FuncMul;

    for (int i = 0; i < 10; ++i) {
        if ((i % 2) == 0)
            printf("func[0] %d\n", func[0](i));
        else
            printf("func[1] %d\n", func[1](i));
    }
}


typedef struct record {
    char name[20];
    int kor;
    int eng;
    int mat;

    void Print() {
        printf("name: %s, kor: %d, eng: %d, mat: %d\n",
            name, kor, eng, mat);
    }
} grade;
// typedef struct record grade;

void InsertData(grade* gr)
{
    // gr[0].name;  // ok, (*(gr + 0)).name == (gr + 0)->name
    // gr->name;    // ok
    // (*gr).name;  // ok
    // *gr.name;    // error, * 보다 . 우선순위 높음
    // *(gr.name);  // error
    // *gr->name;   // ok, * 보다 -> 우선순위 높음, 하나의 문자 지정
    // *(gr->name); // ok
    // (*gr)->name; // error

    // gr[0].name == (*(gr + 0)).name == (gr + 0)->name
    strcpy_s(gr[0].name, sizeof(gr[0].name), "kim");
    gr[0].kor = 93;
    gr[0].eng = 73;
    gr[0].mat = 28;

    strcpy_s((gr + 1)->name, sizeof((gr + 1)->name), "lee");
    (gr + 1)->kor = 39;
    (gr + 1)->eng = 80;
    (gr + 1)->mat = 92;
}

void TestStructPtr()
{
    grade student[2];
    InsertData(student);
    student[0].Print();
    student[1].Print();
}

void OpPriority(grade* gr)
{
    // *gr++.name == *((gr++).name);
    // *++gr.name == *((++gr).name);
    // ++*gr.name == ++(*(gr.name));
    // ++(*gr).name == ++((*gr).name);
    // (*gr)++.name == ((*gr)++).name;
    // *gr.name++ == (*(gr.name)++);
    // *gr.++name == *(gr.(++name));
    // gr++->name == (gr++)->name == (*(gr++)).name;
    // ++gr->name == ++(gr->name) == ++((*gr).name);
    // *++gr->name == *(++gr->name) == *(++(gr->name)) == *(++((*gr).name));
    // (*gr->name) + 1 == (*((*gr).name)) + 1;
}


struct list {
    const char* name;
    int grade;
    struct list* next;
};

struct list* MakeFirstNode()
{
    struct list* data;
    data = (struct list*)malloc(sizeof(struct list));
    data->name = "first";
    data->grade = 0;
    data->next = NULL;
    return data;
}

void ConnectNode(struct list* node)
{
    struct list* data;
    data = (struct list*)malloc(sizeof(struct list));
    data->name = "second";
    data->grade = 75;
    data->next = NULL;
    node->next = data;
}

void PrintNode(struct list* node)
{
    while (1) {
        puts(node->name);
        if (node->next == NULL)
            break;
        node = node->next;
    }

    // for(;node != NULL; node = node->next)
    //     puts(node->name);
}

void ReleaseNode(struct list* node)
{
    while (node != NULL) {
        struct list* next = node->next;
        free(node);
        node = next;
    }
}

void TestStructList()
{
    struct list* node = MakeFirstNode();
    ConnectNode(node);
    PrintNode(node);
    ReleaseNode(node);
}


int main()
{
    TestPtrBasic();
    TestArg2DArray();
    TestStrPtr();
    TestVarAdd();
    TestVoidPtr();
    TestArgTwoPtr();
    TestFuncPtr();
    TestStructPtr();
    TestStructList();

    return 0;
}

