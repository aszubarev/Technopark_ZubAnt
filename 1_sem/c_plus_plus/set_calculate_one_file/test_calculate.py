import subprocess
import random

# subprocess.call("./build.sh")

def set_to_string(from_set):
    string = "["
    for i in from_set:
        string += str(i)
        string += ','

    if string != "[":
        string, result = string[:-1], string[-1]
    string += ']'
    return string


def get_set():
    s = set()
    size = random.randint(2, 2)
    for i in range(size):
        s.add(random.randint(1, 10))
    return s


def get_set_task():
    A = get_set()
    B = get_set()
    C = get_set()
    D = get_set()
    E = get_set()
    F = get_set()
    G = get_set()
    H = get_set()
    I = get_set()
    J = get_set()
    K = get_set()
    L = get_set()
    M = get_set()
    N = get_set()
    O = get_set()
    P = get_set()
    Q = get_set()
    R = get_set()
    S = get_set()
    T = get_set()
    V = get_set()
    W = get_set()
    X = get_set()
    Y = get_set()
    Z = get_set()

    task = "({A} U {B} U {C}) / ( {D} / {E} ^ {F} U {G} U ({H} / ({I} U {J})))".format(A=set_to_string(A),
                                                                                B=set_to_string(B),
                                                                                C=set_to_string(C),
                                                                                D=set_to_string(D),
                                                                                E=set_to_string(E),
                                                                                F=set_to_string(F),
                                                                                G=set_to_string(J),
                                                                                H=set_to_string(H),
                                                                                I=set_to_string(I),
                                                                                J=set_to_string(J))
    task_wolfram = "{A} union ( {D} symmetric difference {E} intersect {F} union {G} union ({H} symmetric difference ({I} union {J})))".format(A=set_to_string(A),
                                                                                                                                                D=set_to_string(D),
                                                                                                                                                E=set_to_string(E),
                                                                                                                                                F=set_to_string(F),
                                                                                                                                                G=set_to_string(J),
                                                                                                                                                H=set_to_string(H),
                                                                                                                                                I=set_to_string(I),
                                                                                                                                                J=set_to_string(J))

    return task, task_wolfram


calculate = "/home/vadim/ZubAnt/TechnoPark/1_sem/c_plus_plus/set_calculate/build-set_calculate-Desktop_Qt_5_7_1_GCC_64bit-Debug/set_calculate"

# calculate = './calculate.out'
test_task, task_wolfram = get_set_task()

print(test_task)
print(task_wolfram)
subprocess.call("echo \"{set}\" | {command}".format(set=test_task, command=calculate), shell=True)
