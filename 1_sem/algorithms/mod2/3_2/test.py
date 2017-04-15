import subprocess
import random


def generate_date(number_live):

    segment_live_set = "{number_live}\n".format(number_live=number_live)
    for i in range(0, number_live):
        day_born = random.randint(1, 31)
        month_born = random.randint(1, 12)
        year_born = random.randint(1900, 2017)

        day_die = random.randint(1, 31)
        month_die = random.randint(1, 12)
        year_die = random.randint(year_born, year_born + 100)

        segment_live = "{day_born} {month_born} {year_born} " \
                       "{day_die} {month_die} {year_die}\n".format(day_born=day_born,
                                                                   month_born=month_born,
                                                                   year_born=year_born,
                                                                   day_die=day_die,
                                                                   month_die=month_die,
                                                                   year_die=year_die)
        segment_live_set += segment_live

    return segment_live_set


def generate_new_test(number_live):
    segment_live_set = generate_date(number_live)
    test_file_data = open("test.txt", 'w')
    test_file_data.write(segment_live_set)
    test_file_data.close()


def testing():
    test = ""
    with open('test.txt', 'r') as test_file:
        for line in test_file:
            test += line

    working_dir = "/home/vadim/ZubAnt/TechnoPark/1_sem/algorithms/mod2/3_2/"
    testing_main = working_dir + "main.out"
    testing_good = working_dir + "main_good.out"

    output_main = working_dir + "output_main"
    output_good = working_dir + "output_good"

    subprocess.call("echo \"{test}\" | {command} > {output_main}".
                    format(test=test, command=testing_main, output_main=output_main), shell=True)
    subprocess.call("echo \"{test}\" | {command} > {output_good}".
                    format(test=test, command=testing_good, output_good=output_good), shell=True)

    output_main_file = open("output_main", 'r')
    output_good_file = open("output_good", 'r')

    size_main = int(output_main_file.readline())
    size_good = int(output_good_file.readline())

    if size_main != size_good:
        print("size_main != size_good")
        output_main_file.close()
        output_good_file.close()
        return None

    main_lines = output_main_file.readlines()
    good_lines = output_good_file.readlines()

    for i in range(0, size_main):
        if main_lines[i] != good_lines[i]:
            print("[{i}]".format(i=i), main_lines[i].rstrip(), " | ", good_lines[i].rstrip())

    output_main_file.close()
    output_good_file.close()


if __name__ == "__main__":

    build_source = True
    gen_new_test = True

    if build_source is True:
        subprocess.call("./build.sh")

    if gen_new_test is True:
        generate_new_test(10)

    testing()


