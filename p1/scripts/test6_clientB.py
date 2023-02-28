#! /usr/bin/env python3

import os
import fs_util
import sys
import test6_clientA as test6  # Simply do not want to do test6_common
'''
This is ClientB.
'''


def run_test():
    signal_name_gen = fs_util.get_fs_signal_name(test6.TEST_CASE_NO)

    cur_signal_name = next(signal_name_gen)
    fs_util.record_test_result(test6.TEST_CASE_NO, 'B',
                               f'START fname:{test6.FNAME}')
    fs_util.wait_for_signal(cur_signal_name)

    # check if file exists
    if not fs_util.path_exists(test6.FNAME):
        fs_util.record_test_result(test6.TEST_CASE_NO, 'B', 'not exist')
        sys.exit(1)
    
    # open file
    fd = fs_util.open_file(test6.FNAME)

    # write some data
    cur_str = fs_util.gen_str_by_repeat('b', 100)
    fs_util.write_file(fd, cur_str)
    fs_util.record_test_result(test6.TEST_CASE_NO, 'B',
                               f'Finish Read and Write of b')


    # send signal to clientA
    last_signal_name = cur_signal_name
    cur_signal_name = next(signal_name_gen)
    fs_util.wait_for_signal(cur_signal_name, last_signal_name=last_signal_name)


    # close file after clientA closes
    fs_util.close_file(fd)


    # done
    fs_util.record_test_result(test6.TEST_CASE_NO, 'B', 'OK')


if __name__ == '__main__':
    run_test()
