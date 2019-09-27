#ifndef POEM_SORT_FINAL_POEM_SORT_FUNCTIONS_H
#define POEM_SORT_FINAL_POEM_SORT_FUNCTIONS_H

#include <string_view>
#include <assert.h>
#include <cstdio>
#include <iostream>

///Function which reads file input_name and puts it to char array
///which buff is pointing to.
/**
 *
 * @param buff pointer to a char array, char** (not nullptr)
 * @param input_name name of the input file, const char* (file should exist)
 * @return size of array with file symbols, size_t
 */
size_t read_file (char **buff, const char* input_name){
    assert (buff);

    FILE *input_file = nullptr;
    input_file = fopen(input_name, "r");
    assert(input_file);

    assert(fseek(input_file, 0L, SEEK_END) == 0);
    int size = ftell(input_file);
    assert(size >= 0);
    assert(fseek(input_file, 0L, SEEK_SET) == 0);

    *buff = (char *) calloc(size + 1, sizeof(char));
    if (fread(*buff, sizeof(char), size + 1, input_file) != size + 1) {
        assert(feof(input_file));
    }
    assert(fclose(input_file) == 0);
    ++size;

    char *cur_buff_symbol = *buff + (size - 2);
    while (size > 1 && *cur_buff_symbol == '\0') {
        --size;
        --cur_buff_symbol;
    }

    return size;
}

///Divides array of char *buff to lines and puts them as string_view to array *poem
/**
 *
 * @param poem pointer to string_view pointer, string_view** (not nullptr)
 * @param buff pointer to char pointer, char** (not nullptr)
 * @param buff_size size of char array, size_t
 * @return size of string_view array, size_t
 */
size_t make_string_array(std::string_view **poem, char **buff, size_t buff_size) {
    assert(buff);
    assert(poem);

    bool is_last_eoln = true;
    int lines_count = 0;
    for (int i = 0; i < buff_size - 1; ++i) {
        if ((*buff)[i] == '\n') {
            (*buff)[i] = '\0';
            if (i > 0 && (*buff)[i-1] != '\0') ++lines_count;
        }
    }

    if ((*buff)[buff_size - 2] != '\0') {
        is_last_eoln = false;
        (*buff)[buff_size - 1] = '\0';
        ++lines_count;
    }

    *poem = (std::string_view *) calloc (lines_count, sizeof(std::string_view));

    if (**buff != '\0') (*poem)[0] = std::string_view(*buff);
    int j = 1;
    for (int i = 1; i < buff_size - is_last_eoln; ++i) {
        if ((*buff)[i - 1] == '\0' && (*buff)[i] != '\0') {
            (*poem)[j] = std::string_view(*buff + i);
            ++j;
        }
    }

    return lines_count;
}

///Compares two strings from beginning (starts from the first letter)
/**
 *
 * @param str_1 first string, string_view
 * @param str_2 second string, string_view
 * @return the result of comparison, bool
 */
bool begin_cmp (std::string_view str_1, std::string_view str_2) {
    int start_1 = 0, start_2 = 0;
    while (start_1 < str_1.size() && !isalpha(str_1[start_1])) ++start_1;
    while (start_2 < str_2.size() && !isalpha(str_2[start_2])) ++start_2;

    int pos = 0;
    while (start_1 + pos < str_1.size() &&
           start_2 + pos < str_2.size() &&
           str_1[start_1 + pos] == str_2[start_2 + pos])
        ++pos;

    if (start_1+pos == str_1.size() && start_2+pos < str_2.size()) return true;
    if (start_1+pos >= str_1.size() || start_2+pos >= str_2.size()) return false;
    return  (str_1[start_1 + pos] < str_2[start_2 + pos]);
}

///Compares two strings from ending (starts from the last letter)
/**
 *
 * @param str_1 first string, string_view
 * @param str_2 second string, string_view
 * @return the result of comparison, bool
 */
bool end_cmp (std::string_view str_1, std::string_view str_2) {
    int end_1 = str_1.size() - 1, end_2 = str_2.size() - 1;
    while (end_1 >= 0 && !isalpha(str_1[end_1])) --end_1;
    while (end_2 >= 0 && !isalpha(str_2[end_2])) --end_2;
    int pos = 0;
    while (end_1 - pos >= 0 &&
           end_2 - pos >= 0 &&
           str_1[end_1 - pos] == str_2[end_2 - pos])
        ++pos;

    if (end_1 - pos == -1 && end_2 - pos >= 0) return true;
    if (end_1 - pos < 0 || end_2 - pos < 0) return false;
    return (str_1[end_1 - pos] < str_2[end_2 - pos]);
}

///Clears the output file
/**
 *
 * @param output_name name of the output file, const char*
 */
void clear_output(const char *output_name) {
    FILE *file = fopen(output_name, "w");
    assert(file);
    assert(fclose(file) == 0);
}

///Prints lines from the poem array to output file
/**
 *
 * @param count count of lines, int
 * @param poem pointer to array of lines, string_view** (not nullptr)
 * @param output_name name of the output file, const char* (file should exist)
 */
void print_lines(size_t count, std::string_view **poem, const char *output_name) {
    assert(poem);
    FILE *file = fopen(output_name, "a");
    assert(file);
    for (int i = 0; i < count; i++) {
        fputs((*poem)[i].data(), file);
        fputc('\n', file);
    }
    fputs("-------------\n", file);
    assert(fclose(file) == 0);
}

///Prints buffer to output file
/**
 *
 * @param size size of buffer, int
 * @param buff pointer to buffer, char**
 * @param output_name name of the output file, const char* (file should exist)
 */
void print_buff(size_t size, char **buff, const char *output_name) {
    assert(buff);
    FILE *file = fopen(output_name, "a");
    assert(file);
    for (int i = 0; i < size; i++) {
        if (i == 0 || (*buff)[i - 1] == '\0') {
            assert(fputs(*buff + i, file) != EOF);
            fputc('\n', file);
            assert(!ferror(file));
        }
    }
    assert(fclose(file) == 0);
}

#endif //POEM_SORT_FINAL_POEM_SORT_FUNCTIONS_H
