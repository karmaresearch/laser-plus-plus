//
// Created by mike on 7/17/18.
//

#include "rule/default_rule_reader.h"

namespace laser {
namespace rule {

DefaultRuleReader::DefaultRuleReader(std::string rule_string)
    : rule_string(std::move(rule_string)) {}

inline void
DefaultRuleReader::syntax_error(std::string const &error_message) const {
    std::string message = "Line: " + std::to_string(line_counter) +
                          " -> Syntax Error: " + error_message;
    const char *exception_message = message.c_str();
    throw util::FormatException(exception_message);
}

void DefaultRuleReader::skip_spaces() {
    input >> std::ws; // eats-up any leadin whitespaces.
}

char DefaultRuleReader::read_next_char() { return input.get(); }

void DefaultRuleReader::read_expected_char(char c) {
    if (read_next_char() != c) {
        std::string error_message = "Expected character = ";
        error_message.push_back(c);
        syntax_error(error_message);
    }
}

char DefaultRuleReader::peek_next_char() { return input.peek(); }

bool DefaultRuleReader::is_next_char_letter() {
    return std::isalpha(peek_next_char());
}

bool DefaultRuleReader::is_next_char_digit() {
    return std::isdigit(peek_next_char());
}

bool DefaultRuleReader::is_next_char_letter_or_digit() {
    return is_next_char_letter() || is_next_char_digit();
}

bool DefaultRuleReader::is_next_char(char c) { return peek_next_char() == c; }

bool DefaultRuleReader::is_next_char_binary_operator() {
    return is_next_char('&') || is_next_char('|');
}

bool DefaultRuleReader::is_next_char_math_operator() {
    return is_next_char('+') || is_next_char('-') || is_next_char('*') 
        || is_next_char('/');
}

bool DefaultRuleReader::is_next_char_comparison_operator() {
    return is_next_char('<') || is_next_char('>') || is_next_char('=') 
        || is_next_char('!');
}

void DefaultRuleReader::parse_eoln() {
    skip_spaces();
    if (!input.eof()) {
        syntax_error("Expecting EOL");
    }
}

std::string DefaultRuleReader::parse_math_sign() {}

std::string DefaultRuleReader::parse_comparison_sign() {}

char DefaultRuleReader::parse_nonzero() {}

char DefaultRuleReader::parse_zero() {}

char DefaultRuleReader::parse_diggit() {}

char DefaultRuleReader::parse_letter() {}

std::string DefaultRuleReader::parse_positive_number() {}

std::string DefaultRuleReader::parse_natural_number() {}

std::string DefaultRuleReader::parse_integer() {}

std::string DefaultRuleReader::parse_float() {}

std::string DefaultRuleReader::parse_identifier() {}

std::string DefaultRuleReader::parse_argument() {}

std::string DefaultRuleReader::parse_float_argument() {}

std::vector<laser::rule::Rule> DefaultRuleReader::parse_program() {
    std::vector<laser::rule::Rule> result;
    std::stringstream program_stream;
    program_stream.str(rule_string);
    std::string line;
    line_counter = 0;
    while (std::getline(program_stream, line, '\n')) {
        line_counter++;
        input.str(std::string()); // clearing input stream - not sure if needed
        input.str(line);
        auto rule = parse_rule();
        result.push_back(rule);
    }
    return result;
}

laser::rule::Rule DefaultRuleReader::parse_rule() {
    auto head = parse_head();
    skip_spaces();
    read_expected_char(':');
    read_expected_char('=');
    auto body = parse_body();
    parse_eoln();
    auto result = laser::rule::Rule(head, body);
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_head() {
    laser::formula::Formula *result;
    skip_spaces();
    if (is_next_char('(')) {
        result = parse_exact_time();
    } else {
        result = parse_predicate_atom();
    }
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_body() {
    return parse_formula();
}

laser::formula::Formula *DefaultRuleReader::parse_formula() {
    laser::formula::Formula *result;
    skip_spaces();
    if (is_next_char('(')) {
        result = parse_complex_formula();
    } else {
        result = parse_binary_formula();
    }
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_complex_formula() {
    read_expected_char('(');
    auto result = parse_formula();
    skip_spaces();
    read_expected_char(')');
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_binary_formula() {
    auto left_term = parse_formula();
    skip_spaces();
    while (is_next_char_binary_operator()) {
        auto binary_formula = parse_binary_operator();
        auto right_term = parse_term();
        binary_formula.add_child(left_term);
        binary_formula.add_child(right_term);
        left_term = binary_formula;
    }
    return left_term;
}

laser::formula::Formula *DefaultRuleReader::parse_binary_operator() {
    laser::formula::Formula *result;
    if (is_next_char('&')) {
        read_expected_char('&');
        read_expected_char('&');
        // result = new Conjunction();
    } else {
        read_expected_char('|');
        read_expected_char('|');
        // result = new Disjunction();
    }
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_term() {
    laser::formula::Formula *result;
    skip_spaces();
    if (is_next_char('(')) {
        result = parser_atom();
    } else if (is_next_char('[')) {
        result = parser_unary_formula();
    } else {
        result = parse_predicate_atom();
    }
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_atom() {
    laser::formula::Formula *result;
    skip_spaces();
    if (is_next_char_math_operator()) {
        result = parser_math_atom();
    } else if (is_next_char_comparison_operator()) {
        result = parser_comparison_atom();
    } else {
        result = parse_predicate_atom();
    }
    return result;

}

laser::formula::Formula *DefaultRuleReader::parse_predicate_atom() {
    
}

laser::formula::Formula *DefaultRuleReader::parse_comparison_atom() {}

laser::formula::Formula *DefaultRuleReader::parse_math_atom() {}

laser::formula::Formula *DefaultRuleReader::parse_usary_formula() {}

laser::formula::Formula *DefaultRuleReader::parse_diamond() {}

laser::formula::Formula *DefaultRuleReader::parse_box() {}

laser::formula::Formula *DefaultRuleReader::parse_negation() {}

laser::formula::Formula *DefaultRuleReader::parse_exact_time() {}

laser::formula::Formula *DefaultRuleReader::parse_time_window() {}

laser::formula::Formula *DefaultRuleReader::parse_tuple_window() {}

std::vector<laser::rule::Rule> DefaultRuleReader::get_rules() {
    return parse_program();
}

} // namespace rule
} // namespace laser
