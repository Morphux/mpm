#!/usr/bin/env python3

from os         import listdir, popen, chdir
from os.path    import isfile, join
import          sys
from subprocess import Popen, PIPE, STDOUT, call

dirs = [
    "base/",
    "config/",
    "make-pkg/",
    "extract/"
]

CMD = ".cmd"
RESULT = ".result"
LINE_LEN=80

total_tests = 0
failed_test = 0

def simple_exec(cmd):
    chdir("../")
    p = Popen(cmd, stdin=PIPE, stdout=PIPE, stderr=STDOUT, shell=True)
    out = p.communicate()[0]
    chdir("./tests")
    return out

def line_diff(str2, str1):
    i = 0

    print("< ", end='')
    while i < len(str1):
        if i < len(str2):
            if str1[i] == str2[i]:
                print(str1[i], end='')
            else:
                print("\033[0;31m" + str1[i] + "\033[0;m", end='')
        else:
                print("\033[0;31m" + str1[i] + "\033[0;m", end='')
        i += 1
    print("\n", end='')

def diff(cmd_result, file_result):
    res = file_result.split('\n')
    cmd = cmd_result.split('\n')

    i = 0
    for line in res:
        if line != cmd[i]:
            line_diff(cmd[i], line)
            if i < len(cmd):
                print("> " + cmd[i])
            else:
                print("> MISSING LINE")
        i += 1

def padding(test_name):
    i = 0;

    print("Testing " + test_name + "... ", end='')
    while i < LINE_LEN - len(test_name) - 5:
        print(' ', end='')
        i = i + 1

def single_test(test_name, __dir):
    global total_tests, failed_test

    padding(test_name)
    with open(__dir + test_name + CMD) as cmd_file:
        cmd = cmd_file.read()
        with open(__dir + test_name + RESULT) as result_file:
            result = result_file.read()
            cmd_result = simple_exec(cmd).decode()
            if (result != cmd_result):
                print("[ \033[1;31mKO\033[0;m ]")
                diff(cmd_result, result)
                failed_test += 1
            else:
                print("[ \033[1;32mOK\033[0;m ]")
    total_tests += 1

def exec_tests(files, __dir):
    for f in files:
        test_name = f
        if (test_name == ""):
            continue
        if (isfile(join(__dir, test_name + RESULT)) == False):
            print("Cannot find result file for test '" + test_name + "'\nQuitting..")
            sys.exit(1)
        single_test(test_name, __dir)

def title(string):
    i = 0
    while i < LINE_LEN / 2 - len(string) + 11:
        print("=", end='')
        i += 1
    print(" " + string + " ", end='')
    while i < LINE_LEN - len(string) + 11:
        print("=", end='')
        i += 1
    print("");

def main():
    for __dir in dirs:
        title(__dir)
        with open(__dir + "order") as order_file:
            exec_tests(order_file.read().split('\n'), __dir)
    print("Results: "+ str(total_tests - failed_test) +" / "+ str(total_tests))
    if (failed_test == 0):
        sys.exit(0)
    sys.exit(1)

main();
