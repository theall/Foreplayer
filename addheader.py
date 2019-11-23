g_header ="""\
/**\r\n
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com\r\n
 *\r\n
 * This program is free software; you can redistribute it and/or\r\n
 * modify it under the terms of the GNU General Public License\r\n
 * as published by the Free Software Foundation; either version 2\r\n
 * of the License, or (at your option) any later version.\r\n
 *\r\n
 * This program is distributed in the hope that it will be useful,\r\n
 * but WITHOUT ANY WARRANTY; without even the implied warranty of\r\n
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\r\n
 * GNU General Public License for more details.\r\n
 *\r\n
 * You should have received a copy of the GNU General Public License\r\n
 * along with this program; if not, write to the Free Software Foundation,\r\n
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.\r\n
 */\
"""
import os
import sys

def main(filename):
    headlines = g_header.split("\r\n")
    try:
        f = open(filename)
    except:
        f = open(filename, encoding='UTF-8')
    lines = f.readlines()
    f.close()
    
    needInsert = False
    if len(lines) > len(headlines):
        for i in range(len(headlines)):
            if headlines[i].strip() != lines[i].strip():
                needInsert = True
                break
    else:
        needInsert = True
    
    if needInsert:
        f = open(filename, 'w')
        f.writelines(headlines)
        f.write("\r\n")
        f.writelines(lines)
        f.close()
    
if __name__=='__main__':
    if len(sys.argv) != 2:
        print("Usage: addheader filename")
        exit()
        
    main(sys.argv[1])
 