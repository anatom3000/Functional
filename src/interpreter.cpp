#include <cmath>
#include <memory>
#include <string>
#include <variant>
#include <vector>

enum class TokenKind {
    Literal,
    Ident,
    Comma,
    LParen,
    RParen,
    Plus,
    Minus,
    Times,
    Divide,
    Pow,
};

struct Token {
    TokenKind kind;
    std::string ident;
    double value;


    std::string format() {
        switch (this->kind) {
            case TokenKind::Literal: return std::to_string(this->value);
            case TokenKind::Ident:   return this->ident;
            case TokenKind::Comma:   return ",";
            case TokenKind::LParen:  return "(";
            case TokenKind::RParen:  return ")";
            case TokenKind::Plus:    return "+";
            case TokenKind::Minus:   return "-";
            case TokenKind::Times:   return "*";
            case TokenKind::Divide:  return "/";
            case TokenKind::Pow:     return "**";
        }
    }
};

using Tokens = std::vector<Token>;

bool is_ident_char(char c, bool allow_digits) {
    return (c == '_')
        || ('A' <= c && c <= 'Z')
        || ('a' <= c && c <= 'z')
        || (allow_digits && ('0' <= c && c <= '9'))
    ;
}

std::variant<Tokens, size_t> tokenize(std::string_view input) {
    Tokens tokens{};

    size_t len = input.length();
    size_t i = 0;
    while (i < len) {
        Token tok{};
        bool add = true;

        switch (input[i]) {
            case ' ':
            case '\t':
                add = false;
            case ',': tok.kind = TokenKind::Comma; break;
            case '(': tok.kind = TokenKind::LParen; break;
            case ')': tok.kind = TokenKind::RParen; break;
            case '+': tok.kind = TokenKind::Plus; break;
            case '-': tok.kind = TokenKind::Minus; break;
            case '/': tok.kind = TokenKind::Divide; break;
            case '*': {
                if (i+1 < input.length() && input[i+1] == '*') {
                    i++;
                    tok.kind = TokenKind::Pow;
                } else {
                    tok.kind = TokenKind::Times;
                }
                break;
            }
            default:
                if (is_ident_char(input[i], false)) {
                    tok.kind = TokenKind::Ident;

                    while (i < len && is_ident_char(input[i], true)) {
                        tok.ident.push_back(input[i]);
                        i++;
                    }
                    i--;
                } else if (input[i] == '.' || ('0' <= input[i] && input[i] <= '9')) {
                    std::string value{};
                    while (i < len && '0' <= input[i] && input[i] <= '9') {
                        value.push_back(input[i]);
                        i++;
                    }
                    if (i < len && input[i] == '.') {
                        value.push_back('.');
                        i++;
                        
                        while (i < len && '0' <= input[i] && input[i] <= '9') {
                            value.push_back(input[i]);
                            i++;
                        }
                    }

                    tok.kind = TokenKind::Literal;
                    tok.value = (value == ".") ? 0.0 : std::stod(value);
                    i--;
                } else {
                    return i;
                }
        }

        if (add) tokens.push_back(tok);
        i++;
    }

    return tokens;
}


std::string format_tokens(Tokens tokens) {
    std::string out = "";
    for (auto tok : tokens) {
        out.append(tok.format());
    }

    return out;
}

enum class ExprKind {
    Literal,
    Variable,
    Application,
    Neg,
    Add,
    Sub,
    Mul,
    Div,
    Pow,
};

struct Expr {
    ExprKind kind;
    // Variable, Application (function name)
    std::string name;
    // Literal
    double value;
    // Add, Sub, Mul, Div, Pow, Neg
    std::unique_ptr<Expr> left;
    // Add, Sub, Mul, Div, Pow
    std::unique_ptr<Expr> right;
    // Application
    std::vector<Expr> args;

    double interpret(double t) {
        switch (this->kind) {
            case ExprKind::Literal: {

                return this->value;
            }
            case ExprKind::Variable: {
                if (this->name == "t") {
                    return t;
                } else {
                    return NAN;
                }
            }
            case ExprKind::Application: {
                if (this->name == "random") {
                    return double(std::rand()) / double(RAND_MAX);
                }
                
                if (this->args.size() < 0) return NAN;
                double arg = this->args[0].interpret(t);
                if (std::isnan(arg)) return NAN;

                if (this->name == "sin") {
                    return std::sin(arg);
                } else if (this->name == "cos") {
                    return std::cos(arg);
                } else if (this->name == "tan") {
                    return std::tan(arg);
                } else if (this->name == "acos") {
                    return std::acos(arg);
                } else if (this->name == "asin") {
                    return std::asin(arg);
                } else if (this->name == "atan") {
                    return std::atan(arg);
                } else if (this->name == "sqrt") {
                    return std::sqrt(arg);
                } else if (this->name == "cbrt") {
                    return std::cbrt(arg);
                } else if (this->name == "exp") {
                    return std::exp(arg);
                } else if (this->name == "ln") {
                    return std::log(arg);
                } else if (this->name == "log10") {
                    return std::log10(arg);
                } else if (this->name == "log2") {
                    return std::log2(arg);
                } else if (this->name == "cosh") {
                    return std::cosh(arg);
                } else if (this->name == "sinh") {
                    return std::sinh(arg);
                } else if (this->name == "tanh") {
                    return std::tanh(arg);
                } else if (this->name == "abs") {
                    return std::abs(arg);
                } else if (this->name == "floor") {
                    return std::floor(arg);
                } else if (this->name == "ceil") {
                    return std::ceil(arg);
                } else if (this->name == "round") {
                    return std::round(arg);
                } else if (this->name == "sgn") {
                    if (arg == 0.0) {
                        return 0.0;
                    } else if (arg > 0.0) {
                        return 1.0;
                    } else {
                        return -1.0;
                    }
                }

                if (this->args.size() < 1) return NAN;
                double arg2 = this->args[1].interpret(t);
                if (std::isnan(arg2)) return NAN;

                if (this->name == "min") {
                    return std::min(arg, arg2);
                } else if (this->name == "max") {
                    return std::max(arg, arg2);
                }
                
                return NAN;
            }
            case ExprKind::Neg: {
                double arg = this->left->interpret(t);
                return -arg;
            }
            case ExprKind::Add: {
                double left = this->left->interpret(t);
                double right = this->right->interpret(t);

                return left + right;
            }
            case ExprKind::Sub: {
                double left = this->left->interpret(t);
                double right = this->right->interpret(t);

                return left - right;
            }
            case ExprKind::Mul: {
                double left = this->left->interpret(t);
                double right = this->right->interpret(t);

                return left * right;
            }
            case ExprKind::Div: {
                double left = this->left->interpret(t);
                double right = this->right->interpret(t);

                if (std::abs(right) < 10e-6) return NAN;

                return left / right;
            }
            case ExprKind::Pow: {
                double left = this->left->interpret(t);
                double right = this->right->interpret(t);

                if (std::isnan(left) || std::isnan(right)) return NAN;

                if (left < 0.0 || (right - std::round(right)) < 1e-6) {
                    right = std::round(right);
                }

                return pow(left, right);
            }
    
        }
    }
};

using ParseResult = std::variant<Expr, std::string>;

#define TRY(var, expr) {\
    ParseResult _try_val = expr;\
    try {\
        var = std::move(std::get<0>(_try_val));\
    } catch (const std::bad_variant_access &e) {\
        return _try_val;\
    }\
}

ParseResult parse_expression(Tokens& tokens);
ParseResult parse_sum(Tokens& tokens, size_t& at);
ParseResult parse_product(Tokens& tokens, size_t& at);
ParseResult parse_pow(Tokens& tokens, size_t& at);
ParseResult parse_primary(Tokens& tokens, size_t& at);

ParseResult parse_expression(Tokens& tokens) {
    size_t at = 0;
    return parse_sum(tokens, at);
}

ParseResult parse_sum(Tokens& tokens, size_t& at) {
    Expr expr;
    TRY(expr, parse_product(tokens, at));

    while (at < tokens.size() && (tokens[at].kind == TokenKind::Plus || tokens[at].kind == TokenKind::Minus)) {
        if (tokens[at].kind == TokenKind::Plus) {
            at++;
            auto right = std::get<0>(parse_product(tokens, at));
            expr = {
                .kind = ExprKind::Add,
                .left = std::make_unique<Expr>(std::move(expr)),
                .right = std::make_unique<Expr>(std::move(right))
            };
        } else if (tokens[at].kind == TokenKind::Minus) {
            at++;
            auto right = std::get<0>(parse_product(tokens, at));
            expr = {
                .kind = ExprKind::Sub,
                .left = std::make_unique<Expr>(std::move(expr)),
                .right = std::make_unique<Expr>(std::move(right))
            };
        }
    }

    return expr;
}

ParseResult parse_product(Tokens& tokens, size_t& at) {
    Expr expr;
    TRY(expr, parse_pow(tokens, at));

    while (at < tokens.size() && (tokens[at].kind == TokenKind::Times || tokens[at].kind == TokenKind::Divide)) {
        if (tokens[at].kind == TokenKind::Times) {
            at++;
            Expr right;
            TRY(right, parse_pow(tokens, at));
            expr = {
                .kind = ExprKind::Mul,
                .left = std::make_unique<Expr>(std::move(expr)),
                .right = std::make_unique<Expr>(std::move(right))
            };
        } else if (tokens[at].kind == TokenKind::Divide) {
            at++;
            Expr right;
            TRY(right, parse_pow(tokens, at));
            expr = {
                .kind = ExprKind::Div,
                .left = std::make_unique<Expr>(std::move(expr)),
                .right = std::make_unique<Expr>(std::move(right))
            };
        }
    }

    return expr;
}

ParseResult parse_pow(Tokens& tokens, size_t& at) {
    Expr expr;
    TRY(expr, parse_primary(tokens, at));

    if (at < tokens.size() && tokens[at].kind == TokenKind::Pow) {
        at++;
        Expr right;
        TRY(right, parse_pow(tokens, at));
        expr = {
            .kind = ExprKind::Pow,
            .left = std::make_unique<Expr>(std::move(expr)),
            .right = std::make_unique<Expr>(std::move(right))
        };
    }

    return expr;
}

ParseResult parse_primary(Tokens& tokens, size_t& at) {
    if (at >= tokens.size()) {
        return std::format("expected expression, found end of string");
    }

    Expr expr;

    switch (tokens[at].kind) {
        case TokenKind::Minus: {
            at++;
            Expr inner;
            TRY(inner, parse_primary(tokens, at));
            expr = {
                .kind = ExprKind::Neg,
                .left = std::make_unique<Expr>(std::move(inner)),
            };
            break;
        }
        case TokenKind::LParen: {
            at++;
            TRY(expr, parse_sum(tokens, at));

            if (!(at < tokens.size() && tokens[at].kind == TokenKind::RParen)) {
                return std::format("expected ')' after parenthesized expression at token {}", at+1);
            }
            at++;
            break;
        }
        case TokenKind::Literal: {
            expr = {
                .kind = ExprKind::Literal,
                .value = tokens[at].value,
            };
            at++;
            break;
        }
        case TokenKind::Ident: {
            if (at+1 < tokens.size() && tokens[at+1].kind == TokenKind::LParen) {
                auto func_name = tokens[at].ident;
                at += 2;

                if (at < tokens.size() && tokens[at].kind == TokenKind::RParen) {
                    expr = { .kind = ExprKind::Application, .name = func_name, .args = {} };
                    at++;
                    break;
                }

                Expr first;
                TRY(first, parse_sum(tokens, at));
                std::vector<Expr> args{};
                args.push_back(std::move(first));
                
                while (at < tokens.size() && tokens[at].kind == TokenKind::Comma) {
                    at++;
                    Expr arg;
                    TRY(arg, parse_sum(tokens, at));
                    args.push_back(std::move(arg));
                }

                if (!(at < tokens.size() && tokens[at].kind == TokenKind::RParen)) {
                    return std::format("expected ')' after function arguments at token {}", at+1);
                }
                at++;

                expr = {
                    .kind = ExprKind::Application,
                    .name = func_name,
                    .args = std::move(args),
                };
            } else {
                expr = {
                    .kind = ExprKind::Variable,
                    .name = tokens[at].ident
                };
                at++;
            }
            break;
        }
        default: {
            return std::format("unexpected token {} at token {}", tokens[at].format(), at+1);
        }
    }

    return expr;
}
