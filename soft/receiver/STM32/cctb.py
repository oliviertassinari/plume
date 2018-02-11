#!/usr/bin/python3

__author__ = 'daybreak'
import sys

def main(out_name, in_names):

    with open(out_name, 'wb') as out_file:
        create(out_file, in_names)



def create(out_file, in_names):

    for in_name in in_names:
        print('Opening %s' % in_name)
        with open(in_name, 'r') as in_file:
            print('.. Success.')
            for line in in_file:
                if line.startswith('#') or line is '\n':
                    continue
                def get_split(line):
                    ret = []
                    for i in line.split():
                        if '#' in i:
                            return ret
                        else:
                            ret.append(i)
                    return ret

                elems = get_split(line)
                type_ = elems[0]

                if type_ is 'w' or type_ is 'r':
                    cmd, ad, reg, val = elems
                elif type_ is '>':
                    cmd, val = elems
                    reg = int(reg, 16) + 1
                    reg = '%x' % reg
                else:
                    continue

                ret = handle(cmd, reg, val)
                #print(cmd, reg, val)
                #print(ret)
                #out_file.write(ret[0])
                #out_file.write(ret[1])
                out_file.write(ret)
                print('wrote : (%s, %s, %s)  -> %s' % (cmd, reg, val, str(ret)))

def handle(cmd, reg, val):
    ret = [0, int(val, 16)]
    if cmd is 'w' or '>':
        ret[0] = (int(reg, 16) * 2) & 0xff
    elif cmd is 'r':
        ret[1] = (int(reg, 16) * 2 + 1) & 0xff
    return bytes(ret)






if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage cctb codec_cfg1 [codec_cfg2, codec_cfg3,...] output")
        exit(0)

    main(sys.argv[-1], sys.argv[1:-1])
    #main('test.c', ['src/conf.codec'])