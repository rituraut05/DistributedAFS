#! /usr/bin/env python3

import os
import fs_util
import sys
import test7_clientA as test7  # Simply do not want to do test7_common
'''
This is ClientB.
'''


def run_test():
    print("hi")
    signal_name_gen = fs_util.get_fs_signal_name(test7.TEST_CASE_NO)

    cur_signal_name = next(signal_name_gen)
    print(f"curr signal name: {cur_signal_name} ")
    fs_util.record_test_result(test7.TEST_CASE_NO, 'B',
                               f'START fname:{test7.FNAME}')
    print(f"test7.FNAME: {test7.FNAME} ")
    fs_util.wait_for_signal(cur_signal_name)

    # first execution, read all-zero file
    if not fs_util.path_exists(test7.FNAME):
        fs_util.record_test_result(test7.TEST_CASE_NO, 'B', 'not exist')
        sys.exit(1)
    
    fd = fs_util.open_file(test7.FNAME)

    last_signal_name = cur_signal_name
    cur_signal_name = next(signal_name_gen)
    fs_util.wait_for_signal(cur_signal_name, last_signal_name=last_signal_name)

    fs_util.close_file(fd)


    # done
    fs_util.record_test_result(test7.TEST_CASE_NO, 'B', 'OK')


if __name__ == '__main__':
    run_test()
