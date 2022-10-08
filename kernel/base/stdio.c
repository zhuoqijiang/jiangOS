#include "kernel/base/string.h"
#include "kernel/base/io.h"
void printf(const char* format, ...)
{
	const int out_str_max = 512;
	const int tmp_str_max = 128;
	char out_str[out_str_max];
	char tmp_str[tmp_str_max];

	memset(out_str, 0, out_str_max);
	memset(tmp_str, 0, tmp_str_max);

	int out_str_index = 0;
	int format_index = 0;

	bool is_format = false;

	char* va_start = (char*)&format + sizeof(&format);
	while (format[format_index]) {
		if (format[format_index] == '%') {
			switch (format[format_index + 1]) {
			case 'd':
				itoas(*((int*)va_start), tmp_str, 10);
				strcat(out_str, tmp_str);
				out_str_index += strlen(tmp_str);
				memset(tmp_str, 0, tmp_str_max);
				va_start += 4;
				is_format = true;
				break;
			case 's':
				strcat(out_str, *((char**)va_start));
				out_str_index += strlen(*((char**)va_start));
				va_start += 4;
				is_format = true;
				break;
			case 'c':
				tmp_str[0] = *va_start;
				strcat(out_str, tmp_str);
				out_str_index += 1;
				tmp_str[0] = 0;
				va_start += 4;
				is_format = true;
				break;
			default:
				is_format = false;
			}
			if (is_format) {
				format_index += 2;
				continue;
			}

		}
		out_str[out_str_index] = format[format_index];
		out_str_index++;
		format_index++;
	}
	dis_str(out_str);
}
