#include <iostream>
#include <string_view>
#include <algorithm>
#include <assert.h>
#include <cstdio>
#include "poem_sort_functions.h"

int main() {

    char *buff = nullptr;
    const char* input_file = "input.txt";
    const char* output_file = "output.txt";
    int poem_size = read_file(&buff, input_file);
    std::string_view *lines = nullptr;
    int count = make_string_array(&lines, &buff, poem_size);

    clear_output(output_file);

    std::sort(lines, lines + count, &begin_cmp);
    print_lines(count, &lines, output_file);

    std::sort(lines, lines + count, &end_cmp);
    print_lines(count, &lines, output_file);

    print_buff(poem_size, &buff, output_file);
    std::free (buff);
    std::free (lines);
    return 0;
}