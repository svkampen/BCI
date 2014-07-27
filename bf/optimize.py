import re
import sys

LOOP_UNROLL_RE = re.compile(r"(\+)(\d+)\[(>(.)(\d*)<-(\d*))\]")

def digits_to_ints(l):
    return [int(d) if d.isdigit() else d for d in l]

def unroll_loop(m):
    m = digits_to_ints(m.groups())
    if m[-1] == '':
        m[-1] = 1
    return ">%s%d<" % (m[0], (m[1]/m[-1])*m[4])

def remove_opposites(m):
    return m.replace("<>", "")

def main():
    subbed = re.sub(LOOP_UNROLL_RE, unroll_loop, sys.argv[1])
    output = remove_opposites(subbed)
    re_inp = re.sub(r"\{((.)(?:\2+))\}(\d+)", lambda m: m.group(1)*int(m.group(3)), output);
    re_inp = re.sub(r"(.)(\d+)", lambda m: m.group(1)*int(m.group(2)), re_inp);
    re_inp = re.sub(r"(.)(\1+)", lambda m: m.group(1)+str(len(m.group(2))+1), re_inp);
    return print(re_inp)

if __name__ == "__main__":
    main()
