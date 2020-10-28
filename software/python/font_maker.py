# -*- coding: windows-1251 -*-
__author__ = 'Trol'
import sys
import os


def map_char(from_code):
    if from_code <= 127:
        return from_code
    #        unicode                        win-1251
    # А (0x410)        Я (0x42F)    -> A (0xC0)      Я (0xDF)
    # а (0x430)        я (0x44F)    -> a (0xE0)      я (0xFF)
    # ё (0x451)                     -> ё (0xB8)
    # Ё (0x401)                     -> Ё (0xA8)
    # © (0xA9)                      -> © (0xA9)
    if 0xC0 <= from_code <= 0xFF:
        return from_code - 0xC0 + 0x410
    if from_code == 0xA8:
        return 0x401
    if from_code == 0xBB:
        return 0x451
    if from_code == 0xA9:
        return 0xA9
    return 0x20


def parse_code_from_str(s):
    begin = '<CHAR CODE="'
    if s.upper().find(begin) < 0:
        return -1
    c = s[s.find(begin)+len(begin):]
    c = c[0:c.find('"')]
    return int(c)


def process(path):
    with open(path) as f:
        content = f.readlines()
    chars = {}
    for s in content:
        code = parse_code_from_str(s)
        if code >= 0:
            chars[code] = s

    name = None
    out_path = os.path.dirname(path) + '/fonts-win1251'
    if not os.path.exists(out_path):
        os.mkdir(out_path)
    out_path += '/' + os.path.basename(path)

    print '  ', path, len(content), ' -> ', out_path
    with open(out_path, "w") as out_file:
        for s in content:
            if s.upper().find('<FONTNAME>') >= 0:
                name = s.strip().replace('<FONTNAME>', '').replace('</FONTNAME>', '').strip()
                print 'Name:', name
            if s.upper().find('<RANGE FROM') >= 0:
                out_file.write('     <RANGE FROM="32" TO="255"/>\x0D\x0A')
                continue
            elif s.upper().find('<CHARS>') >= 0:
                out_file.write(s)
                for c in range(0x20, 0xff+1):
                    uc = map_char(c)
                    res = chars[uc].replace('<CHAR CODE="' + str(uc) + '"', '<CHAR CODE="' + str(c) + '"')
                    out_file.write(res)
                continue
            elif s.upper().find('<CHAR CODE="') < 0:
                out_file.write(s)
                continue





for i in range(1, len(sys.argv)):
    arg = sys.argv[i]
    if os.path.isdir(arg):
        for filename in os.listdir(arg):
            if filename.lower().endswith(".lcd"):
                process(arg + '/' + filename)
    else:
        if os.path.exists(arg) and arg.lower().endswith(".lcd"):
            process(arg)


