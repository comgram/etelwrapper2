from subprocess import check_output

out = check_output("ctags -x --c-types=f ./interface.c", shell=True).decode("utf-8")

fp = open("interface.h", "w")
fp.write("#ifndef _ETELINTERFACEH_\n")
fp.write("#define _ETELINTERFACEH_\n")
fp.write("\n")
fp.write("#include <conio.h>\n")
fp.write("#include <dsa40.h>\n")
fp.write("#include <stdio.h>\n")
fp.write("#include <math.h>\n")
fp.write("#include <stdio.h>\n")
fp.write("#include <windows.h>\n")
fp.write("\n")

body = []
fp2 = open("cwrapper.pxd", "w")
def_str = "ctypedef long long __int64\n"
body.append(def_str)
fp2.write(def_str)
def_str1 = 'cdef extern from "interface.h":\n'
body.append(def_str1)
fp2.write(def_str)

fn = out.split("\r\n")

for f in fn[1:]:
    if f == "":
        continue

    temp = f.split("./interface.c")[1].strip()

    fp.write(temp + ";\n")

    if "(void)" in temp:
        temp = temp.replace("(void)", "()")

    fp2.write("    " + temp + "\n")
    body.append("    " + temp + "\n")

fp.write("#endif\n")
fp.close()
fp2.close()

fp3 = open("wrapper.pyx", "r")

pyx_lines = fp3.readlines()
pyx_line_amount = len(pyx_lines)
pyx_header_start = 0
pyx_header_end = 0
for i, line in enumerate(pyx_lines):
    if "header start" in line:
        pyx_header_start = i
    if "header end" in line:
        pyx_header_end = i
        break

print(pyx_header_start, pyx_header_end, pyx_line_amount)
fp3.close()

# fp4 = open("cwrapper.pxd", 'r')
# header_lines = fp4.readlines()
# fp4.close()
header_lines = body
all_lines = (
    pyx_lines[: pyx_header_start + 1] + header_lines + pyx_lines[pyx_header_end:]
)
fp5 = open("wrapper.pyx", "w")
fp5.writelines(all_lines)
fp5.close()

print("Extract done.")
