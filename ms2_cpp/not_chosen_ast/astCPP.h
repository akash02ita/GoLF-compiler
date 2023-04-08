/******************************
 * Shankar Ganesh
 * AST using Classes for GoLF
 * Compile with -std=c++17
 *****************************/

#ifndef AST_HPP
#define AST_HPP

#include <type_traits>
#include <string>
#include <string_view>
#include <iostream>
#include <string>
#include <vector>

// This file just contained forward declarations of
// all the classes. (the commented list below)
// #include "fwd.hpp"

#define INDENT_CHAR ' '

enum Oper 
{
    ADD, SUB, MULT, DIV, MOD,
    LT, GT, LTE, GTE, EQ, NEQ,
    AND, OR
};

enum TypeName
{
    INT, BOOL, STR, VOID
};

// class AST;
//     class Prog;
//
//     class Stmt;
//         class Block;
//         class IfStmt;
//         class AssnStmt;
//         class EmptyStmt;
//         class DeclStmt;
//         class ReturnStmt;
//         class BreakStmt;
//         class ForStmt;
//         class ExprStmt;
//
//     class Expr;
//         class Id;
//         class TypeId;
//         class BasicLit;
//         class UnaryExp;
//         class BinaryExp;
//         class FuncCall;
//         class Actual;
//         class Actuals;
//
//     class Decl;
//         class FuncDecl;
//         class VarDecl;
//         class ParamDecl;
//         class Params;
//         class Signature;

template<typename Base, typename T>
inline bool isInstanceOf(const T*)
{
    return std::is_base_of<Base, T>::value;
}


inline std::string opToString(Oper op)
{
    switch (op) 
    {
    case ADD:       return "+";
    case SUB:       return "-";
    case MULT:      return "*";
    case DIV:       return "/";
    case MOD:       return "%";
    case LT:        return "<";
    case GT:        return ">";
    case LTE:       return "<=";
    case GTE:       return ">=";
    case EQ:        return "==";
    case NEQ:       return "!=";
    default:        return "";
    }
}

inline std::string typeToStr(TypeName type)
{
    switch (type) 
    {
    case INT:       return "INT";
    case BOOL:      return "BOOL";
    case STR:       return "STR";
    case VOID:      return "VOID";
    default:        return "";
    }
}

class AST
{
    public:
        AST() : m_label("") {}

        virtual ~AST()
        {
            for (auto child : m_children)
                delete child;

            m_children.clear();
        }

        virtual void print(int INDENT=0) = 0;

        virtual void addChild(AST *child) 
        {
            m_children.push_back(child);
        }

        const std::vector<AST*>& getChildren() const
        {
            return m_children;
        }

    protected:

        std::string m_label;
        std::vector<AST*> m_children;

        int m_lineno;
};


/**********************************************
 * PROGRAM 
**********************************************/

class Prog : public AST
{
    public:
        Prog(std::string filename) 
        {
            m_label = "File: " + filename;
        }

        void print(int INDENT=0) override
        {
            std::cout << "Program: " << m_label << '\n'; 
            INDENT++;
            for (auto &child : m_children)
                child->print(INDENT);
            INDENT--;
        }
};

/**********************************************
 * EXPRESSION
**********************************************/

class Expr : public AST
{
    public:
        virtual void print(int INDENT=0) override
        {
            std::cerr << "Expr Node not printable.\n";
        }

};

class Id : public Expr
{
    public:
        Id(const char* id)
        {
            m_val = id;
            m_label = "ID; Value: " + m_val;
        }

        Id(const char* id, int line)
        {
            m_val = id;
            m_lineno = line;
            m_label = "ID; Value: " + m_val + "; Line: " + std::to_string(line);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
        }

        std::string_view getVal() const
        {
            return m_val;
        }

    private:
        std::string m_val;
};


class TypeId : public Expr 
{
    public:
        TypeId(const char* id, int line)
        {
            m_val = id;
            m_lineno = line;
            m_label = "TypeID; Value: " + m_val + "; Line: " + std::to_string(line);
        }

        TypeId(const char* id)
        {
            m_val = id;
            m_label = "TypeID; Value: " + m_val;
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
        }

        std::string_view getVal() const
        {
            return m_val;
        }

    private:
        std::string m_val;
};


class BasicLit : public Expr
{
    public:
        explicit BasicLit(int val, int line) 
        {
            ival = val;
            m_type = TypeName::INT;
            m_lineno = line;
            m_label = "NUM; Value: " + std::to_string(ival) + "; Line: " + std::to_string(line);
        }

        explicit BasicLit(const char* val, int line)
        {
            sval = val;
            m_type = TypeName::STR;
            m_lineno = line;
            m_label = "STR; Value: " + sval + "; Line: " + std::to_string(line);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
        }

        std::string_view getStr() const { return sval; }

        int getInt() const { return ival; }

        TypeName getType() const { return m_type; }

    private:
        int ival;
        std::string sval;

        TypeName m_type;
};


class UnaryExp : public Expr
{
    public:
        UnaryExp(Oper op, Expr *expr, int line)
        {
            m_op = op;
            m_expr = expr;
            m_lineno = line;
            m_label = "UOp: " + opToString(m_op) + "; Line: " + std::to_string(line);
            addChild(m_expr);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_expr->print(INDENT);
            INDENT--;
        }

        Oper getOp() const { return m_op; }

    private:
        Oper m_op;
        Expr* m_expr;
};


class BinaryExp : public Expr
{
    public:
        BinaryExp(Oper op, Expr* lhs, Expr* rhs, int line)
        {
            m_lineno = line;
            m_op = op;
            m_lhs = lhs;
            m_rhs = rhs;
            m_label = "Op: " + opToString(m_op) + "; Line: " 
                + std::to_string(m_lineno);
            addChild(m_lhs);
            addChild(m_rhs);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_lhs->print(INDENT);
            m_rhs->print(INDENT);
            INDENT--;
        }

        Oper getOp() const { return m_op; }

    private:
        Oper m_op;
        Expr* m_lhs;
        Expr* m_rhs;
};


class FuncCall : public Expr
{
    public:
        FuncCall(Expr* funcId, Expr* args)
        {
            m_id = funcId;
            m_args = args;
            m_label = "FuncCall;";
            addChild(m_id);
            addChild(m_args);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_id->print(INDENT);
            m_args->print(INDENT);
            INDENT--;
        }

    private:
        Expr* m_id;
        Expr* m_args;
};


class Actual : public Expr
{
    public:
        Actual(Expr * expr)
        {
            m_expr = expr;
            m_label = "Actual;";
            addChild(m_expr);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_expr->print(INDENT);
            INDENT--;
        }

    private:
        Expr* m_expr;
};


class Actuals : public Expr
{
    public:
        Actuals()
        {
            m_label = "Actuals;";
        }

        void addActual(Actual* arg)
        {
            addChild(arg);
        }

        void print(int INDENT=0) override 
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            for (auto &child : m_children)
                child->print(INDENT);
            INDENT--;
        }
};


/**********************************************
 * STATEMENTS 
**********************************************/

class Stmt : public AST
{
    public:
        Stmt() : m_next(nullptr) {}

        void append(Stmt* stmt1, Stmt* stmt2)
        {
            if (!stmt1->hasNext())
            {
                stmt1->setNext(stmt2);
                return;
            }
            Stmt* lastStmt = stmt1->getNext();
            while (lastStmt->hasNext()) lastStmt = lastStmt->getNext();
            lastStmt->setNext(stmt2);
        }

        virtual void print(int INDENT=0) override
        {
            std::cerr << "Stmt not printable.\n";
        }

        Stmt* getNext() { return m_next; }

        void setNext(Stmt* next)
        {
            if (!m_next) m_next = next;
        }

        bool hasNext() { return m_next != nullptr; }

    private:
        Stmt* m_next;
};

class Block : public Stmt
{
    public:

        Block(Stmt* body)
        {
            m_label = "Block";  
            m_body = body;
            addChild(m_body);

            // Only one statement has been added as the 
            // child of this body. Get next statements (siblings)
            // and add them as children.
            // This is not really necessary but it does offer two ways of accessing
            // the statement nodes.
            auto temp = m_body;
            while (temp->hasNext())
            {
                temp = m_body->getNext();
                addChild(temp);
            }
        }

        Block()
        {
            m_label = "Block";
        }

        void addStmt(Stmt* stmt)
        {
            if (!m_body) m_body = stmt;
            else m_body->append(m_body, stmt);
            addChild(stmt);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            if (!m_body) return;
            INDENT++;
            for (auto &child : m_children)
                child->print(INDENT);
            INDENT--;
        }

    private:
        // Contains the first statement
        // The others can either be accessed using next() of m_body or 
        // using m_children
        Stmt* m_body;
};

class IfStmt : public Stmt
{
    public:
        IfStmt(Expr* cond, Block* ifBlock, Stmt* elseBlock, int line)
        {
            m_ifBlock = ifBlock;
            m_elseBlock = elseBlock;
            m_condition = cond;
            m_lineno = line;
            m_label = "IfElseStmt; Line: " + std::to_string(m_lineno);
            addChild(m_condition);
            addChild(m_ifBlock);
            addChild(m_elseBlock);
        }

        IfStmt(Expr* cond, Block* ifBlock, int line)
        {
            m_ifBlock = ifBlock;
            m_elseBlock = nullptr;
            m_condition = cond;
            m_lineno = line;
            m_label = "IfStmt; Line: " + std::to_string(m_lineno);
            addChild(m_condition);
            addChild(m_ifBlock);
        }


        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_condition->print(INDENT);
            m_ifBlock->print(INDENT);
            if (m_elseBlock) m_elseBlock->print(INDENT);
            INDENT--;
        }

    private:
        Block* m_ifBlock;
        Stmt* m_elseBlock;
        Expr* m_condition;
};

class AssnStmt : public Stmt
{
    public:
        AssnStmt(Expr* lhs, Expr* rhs, int line)
        {
            m_lhs = lhs;
            m_rhs = rhs;
            m_lineno = line;
            m_label = "AssnStmt; Line: " + std::to_string(m_lineno);
            addChild(m_lhs);
            addChild(m_rhs);
        }

        void print(int INDENT=0) override 
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_lhs->print(INDENT);
            m_rhs->print(INDENT);
            INDENT--;
        }

    private:
        Expr* m_lhs;
        Expr* m_rhs;
};

class EmptyStmt : public Stmt
{
    public:

        EmptyStmt()
        {
            m_label = "EmptyStmt;";
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
        }
};



class ReturnStmt : public Stmt
{
    public:
        ReturnStmt(int line)
        {
            m_lineno = line;
            m_expr = nullptr;
            m_label = "ReturnStmt; Line: " + std::to_string(line);
        }

        ReturnStmt(Expr* expr, int line)
        {
            m_lineno = line;
            m_expr = expr;
            m_label = "ReturnStmt; Line: " + std::to_string(line);
            addChild(m_expr);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            if (m_expr) m_expr->print(INDENT);
            INDENT--;
        }

    private:
        Expr* m_expr;

};

class BreakStmt : public Stmt
{
    public:
        BreakStmt(int line)
        {
            m_lineno = line;
            m_label = "BreakStmt; Line: " + std::to_string(m_lineno); 
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
        }
};

class ForStmt : public Stmt
{
    public:
        ForStmt(Expr* cond, Block* body, int line)
        {
            m_cond = cond;
            m_body = body;
            m_lineno = line;
            m_label = "ForStmt; Line: " + std::to_string(m_lineno);
            addChild(m_cond);
            addChild(m_body);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_cond->print(INDENT);
            m_body->print(INDENT);
            INDENT--;
        }


    private:
        Expr* m_cond;
        Block* m_body;

};


class ExprStmt : public Stmt
{
    public:

        ExprStmt(Expr* expr)
        {
            m_expr = expr;
            m_label = "ExprStmt;";
            addChild(m_expr);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_expr->print(INDENT);
            INDENT--;
        }

    private:
        Expr* m_expr;
};


/**********************************************
 * DECLARATIONS
**********************************************/

class Decl : public AST
{
    public:
        Decl() = default;

        void print(int INDENT=0) override
        {
            std::cerr << "Declaration not printable\n";
        }
};


class VarDecl : public Decl
{
    public:
        VarDecl(Id* varName, TypeId* type, int line)
        {
            m_typeid = type;
            m_id = varName;
            m_lineno = line;
            m_label = "VarDecl; Line: " + std::to_string(m_lineno);
            addChild(m_id);
            addChild(m_typeid);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_id->print(INDENT);
            m_typeid->print(INDENT);
            INDENT--;
        }

    private:
        TypeId* m_typeid;
        Id* m_id;
};

class ParamDecl : public Decl
{
    public:
        ParamDecl(Id* name, TypeId* type) 
        {
            m_id = name;
            m_type = type;
            addChild(m_id);
            addChild(m_type);
            m_label = "Param;";
        }

        void print(int INDENT=0) override 
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_id->print(INDENT);
            m_type->print(INDENT);
            INDENT--;
        }

    private:
       Id* m_id;
       TypeId* m_type;
};

class Params : public Decl
{
    public:
        Params()
        {
            m_label = "Params;";
        }

        void addParam(ParamDecl* argDecl)
        {
            addChild(argDecl);
        }

        void print(int INDENT=0) override 
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            for (auto &child : m_children)
                child->print(INDENT);
            INDENT--;
        }
};


class Signature : public Decl
{
    public:
        Signature(TypeId* type, Params* param)
        {
            m_typeid = type;
            m_params = param;
            m_label = "Signature;";
            addChild(m_typeid);
            addChild(m_params);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_typeid->print(INDENT);
            m_params->print(INDENT);
            INDENT--;
        }

    private:
        TypeId* m_typeid;
        Params* m_params;
};


class FuncDecl : public Decl
{
    public:
        FuncDecl(Id* funcName, Signature* sig, Stmt* body, int line)
        {
            m_id = funcName;
            m_lineno = line;
            m_sig = sig;
            m_body = body;
            m_label = "FuncDecl; Line: " + std::to_string(m_lineno);
            addChild(m_id);
            addChild(m_sig);
            addChild(m_body);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_id->print(INDENT);
            m_sig->print(INDENT);
            m_body->print(INDENT);
            INDENT--;
        }

    private:
        Id* m_id;
        Signature* m_sig;
        Stmt* m_body;
};


class DeclStmt : public Stmt
{
    public:
        DeclStmt(Decl* decl)
        {
            m_decl = decl;
            m_label = "DeclStmt;";
            addChild(m_decl);
        }

        void print(int INDENT=0) override
        {
            std::cout << std::string(INDENT*4, INDENT_CHAR);
            std::cout << m_label << '\n';
            INDENT++;
            m_decl->print(INDENT);
            INDENT--;
        }

    private:
        Decl* m_decl;
};

#endif 