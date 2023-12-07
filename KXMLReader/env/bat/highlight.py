import chardet
import codecs
import os
import sys
import argparse 
from pygments import highlight
from pygments.lexers import PythonLexer
from pygments.lexers.c_cpp import CppLexer
from pygments.formatters import HtmlFormatter
from pygments.style import Style
from pygments.token import Token, Comment, Keyword, Name, String, Number, Operator

# 获取main.py文件的路径
# current_path = os.path.dirname(os.path.abspath(__file__))

# 将当前路径添加到Python解释器的搜索路径中
# sys.path.append(current_path)

parser = argparse.ArgumentParser(description = 'highlight')  
parser.add_argument('file_path', help='File paths that need to be highlightede')  
parser.add_argument('background', help='Background color')  
parser.add_argument('font_size', help='Font size')  
parser.add_argument('current_path', help='Export html file path')  
args = parser.parse_args() 

file_path = args.file_path  
background = args.background  
font_size = args.font_size  
current_path = args.current_path  
output_htmlfile_path = os.path.join(current_path, "res.html") 
#output_htmlfile_path = current_path + r"\res.html"

def read_file(file_path):
    with open(file_path, 'rb') as file:
        raw_data = file.read()
        result = chardet.detect(raw_data)
        encoding = result['encoding']

    if encoding == 'GB2312':
        with codecs.open(file_path, 'r', encoding='gb2312') as file:
            code = file.read()
    elif encoding.lower().replace('-', '') == 'utf8bom':
        with codecs.open(file_path, 'r', encoding='utf-8-sig') as file:
            code = file.read()
    else:
        with open(file_path, 'r', encoding='utf-8') as file:
            code = file.read()
    return code

def get_lexer(file_path):
    file_extension = file_path.split(".")[-1].lower()
    if file_extension == "py":
        lexer = PythonLexer
    elif file_extension in ["cpp", "h"]:
        lexer = CppLexer()
    else:
        lexer = CppLexer()
    return lexer

def save_highlighted_code(output_htmlfile_path, highlighted_code, style_defs):
    with open(output_htmlfile_path, "w") as writefile:
        writefile.write("")
    with open(output_htmlfile_path, 'w', encoding='utf-8') as file:
        file.write('<style>{}</style>'.format(style_defs))
        file.write(highlighted_code)
    print('Highlighted code saved to {}'.format(output_htmlfile_path))

class dark_style(Style):
    styles = {
        Token:             '',
        Comment:           'italic #6A9955',   #注释:斜体，深绿色
        Keyword:           'bold #00FFFF',     #关键字:粗体,荧光蓝
        Name:              '#ffffff',          #其余代码：白色
        Name.Class:        'bold #00FF00',     #类名：粗体，亮绿色
        Name.Function:     '#FFFF00',          #函数名：亮黄色
        String:            '#FFA500',          #字符串：橙色
        Number:            '#DCDCDC',          #数字：银灰色
        Operator:          '#FFFFFF'           #操作符：纯白色
    }

def high_light_text(file_path, output_htmlfile_path, background, font_size):
    code = read_file(file_path)
    lexer = get_lexer(file_path)
    if background == 'black':
        formatter = HtmlFormatter(style=dark_style, noclasses=True, cssstyles='background-color:#000000; color:#ffffff;')
        style_defs = 'body {background-color: #000000; color: #ffffff; font-size: ' + font_size + ';}'  #黑色背景颜色,字体大小
    elif background == 'default':
        formatter = HtmlFormatter()
        style_defs = 'body {font-size: ' + font_size + ';}'

    #增加行列号
    code_lines = code.split('\n')
    max_line_number_digits = len(str(len(code_lines)))  #获取最大行的位数，以便对齐
    code_with_line_numbers = []
    for i, line in enumerate(code_lines):
        line_number = str(i+1).rjust(max_line_number_digits)  #行号对齐
        code_with_line_numbers.append(f'{line_number}  {line}')
    highlighted_code = highlight('\n'.join(code_with_line_numbers), lexer, formatter)
    style_defs += formatter.get_style_defs('.highlight')
    save_highlighted_code(output_htmlfile_path, highlighted_code, style_defs)

high_light_text(file_path, output_htmlfile_path, background, font_size)