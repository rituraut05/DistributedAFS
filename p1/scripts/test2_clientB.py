#! /usr/bin/env python3

import os
import fs_util
import sys
import test2_clientA as test  # Simply do not want to do test_common
'''
This is ClientB.
'''


"""

A.          B
open
write
close
            open
            read
kill     
            write
            close

"""

def run_test():
    signal_name_gen = fs_util.get_fs_signal_name(test.TEST_CASE_NO)

    cur_signal_name = next(signal_name_gen)
    print(cur_signal_name)
    fs_util.record_test_result(test.TEST_CASE_NO, 'B',
                               f'START fname:{test.FNAME}')
    fs_util.wait_for_signal(cur_signal_name, True)
    print("Here")

    # first execution, read all-zero file
    if not fs_util.path_exists(test.FNAME):
        print(f"{test.FNAME} does not exixt")
        fs_util.record_test_result(test.TEST_CASE_NO, 'B', 'not exist')
        sys.exit(1)
#open, read
    fd = fs_util.open_file(test.FNAME)
    read_str = fs_util.read_file(fd, 100, 0)
    print(read_str)
    if len(read_str) != 100:
        fs_util.record_test_result(test.TEST_CASE_NO, 'B',
                                   f'read_len:{len(read_str)}')
        sys.exit(1)
    for rc in read_str:
        if rc != '0':
            fs_util.record_test_result(test.TEST_CASE_NO, 'B',
                                       f'read_str:{read_str}')
            sys.exit(1)


    last_signal_name = cur_signal_name
    cur_signal_name = next(signal_name_gen)
    fs_util.wait_for_signal(cur_signal_name, last_signal_name=last_signal_name)

# AFS client gets killed by A
    fs_util.record_test_result(test.TEST_CASE_NO, 'B',
                               f'After client killed by A')



#write, close    
    cur_str = fs_util.gen_str_by_repeat('c', 100)
    fs_util.record_test_result(test.TEST_CASE_NO, 'B',
                               f'new write')
    fs_util.record_test_result(test.TEST_CASE_NO, 'B',
                               f'Write client killed by B {fs_util.write_file(fd, cur_str, start_off=0)}')
    fs_util.close_file(fd)
    fs_util.record_test_result(test.TEST_CASE_NO, 'B',
                               f'Finish Read and Write of b')
    
    

if __name__ == '__main__':
    run_test()
