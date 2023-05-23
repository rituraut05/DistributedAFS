#! /usr/bin/env python3

import os
import fs_util
import sys
import test4_clientA as test4  # Simply do not want to do test4_common
'''
This is ClientB.
'''


def run_test():
    signal_name_gen = fs_util.get_fs_signal_name(test4.TEST_CASE_NO)

    cur_signal_name = next(signal_name_gen)
    print(f"curr signal name: {cur_signal_name} ")
    fs_util.record_test_result(test4.TEST_CASE_NO, 'B',
                               f'START fname:{test4.FNAME}')
    print(f"test4.FNAME: {test4.FNAME} ")
    fs_util.wait_for_signal(cur_signal_name, True)

    # first execution, read all-zero file
    if not fs_util.path_exists(test4.FNAME):
        print(f"{test4.FNAME} does not exixt")
        fs_util.record_test_result(test4.TEST_CASE_NO, 'B', 'not exist')
        sys.exit(1)
    
    fs_util.delete_file(test4.FNAME)

    fs_util.record_test_result(test4.TEST_CASE_NO, 'B',
                               f'Deleted file:{test4.FNAME}')
    fs_util.record_test_result(test4.TEST_CASE_NO, 'B', 'OK')


if __name__ == '__main__':
    run_test()
