#ifndef CELGEBRA_MELGEBRA_HPP
#define CELGEBRA_MELGEBRA_HPP

#include <cstring>
#include <cmath>
#include <tice.h>

#include <cstdio>

void float2str(float value, char *str) {
    real_t tmp_real = os_FloatToReal(value);
    os_RealToStr(str, &tmp_real, 0, 2, 2);
}

namespace MGB {
    // operable object
    class Object {
    public:
        virtual char *string() = 0;
    };

    // abstract symbol
    class Symbol : Object {
    public:
        char *symbol{};

        char *string() override {
            return symbol;
        }
    };

    class Num : Object {
        double num{};

        char *string() override {
            static char str[14];
            float2str(num, str);
            return str;
        }
    };

    // operation that can be performed
    class Operation : public Object {
    public:
        // true for things like multiplication and addition, false for things like sin
        bool is_exact{};
        char *name{};
    };

    // operation with 1 arg
    class Operation1 : Operation {
    public:
        // perform operation on doubles
        virtual double op(double arg1) = 0;

        Object *arg1{};
    };

    // operation with 2 args
    class Operation2 : Operation {
    public:
        // perform operation on doubles
        virtual double op(double arg1, double arg2) = 0;

        Object *arg1{};
        Object *arg2{};
    };

    // single char style operation (like x * y)
    class Operator : public Operation2 {
        char *opchars{};

        char *string() override {
            // get length of new string
            char *str1 = arg1->string();
            char *str2 = arg2->string();
            size_t newlen = strlen(str1) +
                            1 + //space
                            strlen(str2) +
                            1 + //space
                            strlen(opchars) +
                            1; //space
            // allocate memory for string
            char *str = static_cast<char *>(malloc(newlen));
            // build string
            strcat(str, str1);
            strcat(str, " ");
            strcat(str, opchars);
            strcat(str, " ");
            strcat(str, str2);
            return str;
        }
    };

    // function style operation (like sin(x))
    class Function : Operation {
    public:
        char *funcname{};
    };

    // Function that takes 1 arg
    class Function1 : Function, Operation1 {
    public:
        char *string() override {
            // get length of new string
            char *str1 = arg1->string();
            size_t newlen = strlen(funcname) +
                            1 + //(
                            strlen(str1) +
                            1 + //)
                            1; //null
            // allocate memory for string
            char *str = static_cast<char *>(malloc(newlen));
            // build string
            strcat(str, funcname);
            strcat(str, "(");
            strcat(str, str1);
            strcat(str, ")");
            return str;
        }
    };

    // Function that takes 2 args
    class Function2 : Function, Operation2 {
    public:
        char *string() override {
            // get length of new string
            char *str1 = arg1->string();
            char *str2 = arg2->string();
            size_t newlen = strlen(funcname) +
                            1 + //(
                            strlen(str1) +
                            1 + //,
                            strlen(str2) +
                            1 + //)
                            1; //null
            // allocate memory for string
            char *str = static_cast<char *>(malloc(newlen));
            // build string
            strcat(str, funcname);
            strcat(str, "(");
            strcat(str, str1);
            strcat(str, ",");
            strcat(str, str2);
            strcat(str, ")");
            return str;
        }
    };

    class Op_Add : public Operator {
    public:
        bool is_exact = true;
        const char *name = "Add";
        const char *opchars = "+";

        double op(double arg1, double arg2) override {
            return arg1 + arg2;
        }
    };

    class Op_Subtract : public Operator {
    public:
        bool is_exact = true;
        const char *name = "Subtract";
        const char *opchars = "-";

        double op(double arg1, double arg2) override {
            return arg1 - arg2;
        }
    };


    class Op_Multiply : public Operator {
    public:
        bool is_exact = true;
        const char *name = "Multiply";
        const char *opchars = "*";

        double op(double arg1, double arg2) override {
            return arg1 * arg2;
        }
    };


    class Op_Divide : public Operator {
    public:
        bool is_exact = false;
        const char *name = "Divide";
        const char *opchars = "/";

        double op(double arg1, double arg2) override {
            return arg1 / arg2;
        }
    };

    class Op_Power : public Operator {
    public:
        bool is_exact = false;
        const char *name = "Power";
        const char *opchars = "^";

        double op(double arg1, double arg2) override {
            return pow(arg1, arg2);
        }
    };

}

#endif //CELGEBRA_MELGEBRA_HPP
