#! /usr/bin/env python3

import os
import fs_util
import sys
import test5_clientA as test5  # Simply do not want to do test5_common
'''
This is ClientB.
'''


def run_test():
    signal_name_gen = fs_util.get_fs_signal_name(test5.TEST_CASE_NO)

    cur_signal_name = next(signal_name_gen)
    print(f"curr signal name: {cur_signal_name} ")
    fs_util.record_test_result(test5.TEST_CASE_NO, 'B',
                               f'START fname:{test5.FNAME}')
    print(f"test5.FNAME: {test5.FNAME} ")
    fs_util.wait_for_signal(cur_signal_name, True)

    # first execution, read all-zero file
    if not fs_util.path_exists(test5.FNAME):
        print(f"{test5.FNAME} does not exixt")
        fs_util.record_test_result(test5.TEST_CASE_NO, 'B', 'not exist')
        sys.exit(1)
    
    fs_util.delete_file(test5.FNAME)

    fs_util.record_test_result(test5.TEST_CASE_NO, 'B',
                               f'Deleted file:{test5.FNAME}')
    fs_util.record_test_result(test5.TEST_CASE_NO, 'B', 'OK')


if __name__ == '__main__':
    run_test()
