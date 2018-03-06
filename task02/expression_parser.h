#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include "input_parser.h"

#define queue_data_type_t lexem_t
#include "queue.h"

parsing_error_t parse_expression(queue_t *expression, vector_t lexems);

#endif //EXPRESSION_PARSER_H
