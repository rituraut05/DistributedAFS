#! /usr/bin/env python3

import os
import fs_util
import sys
import time
'''
This is ClientA.
'''

cs739_env_vars = [
    'CS739_CLIENT_A', 'CS739_CLIENT_B', 'CS739_SERVER', 'CS739_MOUNT_POINT', 'CS739_MOUNT_POINT2'
]
ENV_VARS = {var_name: os.environ.get(var_name) for var_name in cs739_env_vars}
for env_var in ENV_VARS.items():
    print(env_var)
    assert env_var is not None
TEST_DATA_DIR = ENV_VARS['CS739_MOUNT_POINT'] + '/test_consistency'
FNAME = f'{TEST_DATA_DIR}/case6'
print(TEST_DATA_DIR)
TEST_CASE_NO = 6


"""
TEST CASE 6 - LAST CLOSE WINS
A       B
OPEN
        OPEN
        WRITE
WRITE
CLOSE
        CLOSE
OPEN
READ
"""



def run_test():
    host_b = ENV_VARS['CS739_CLIENT_B']
    print(host_b)
    assert fs_util.test_ssh_access(host_b)
    
    signal_name_gen = fs_util.get_fs_signal_name(TEST_CASE_NO)

    if not fs_util.path_exists(TEST_DATA_DIR):
        fs_util.mkdir(TEST_DATA_DIR)

    # init
    if not fs_util.path_exists(FNAME):
        fs_util.create_file(FNAME)


    # open file
    fd = fs_util.open_file(FNAME)


    # time for client_b to work
    cur_signal_name = next(signal_name_gen)
    fs_util.start_another_client(host_b, 6, 'B', cur_signal_name)

    # wait until client_b finish
    print(f"curr_signal_name: {cur_signal_name}")
    while True:
        removed = fs_util.poll_signal_remove(host_b, cur_signal_name)
        if removed:
            break
        time.sleep(1)
    print('Clientb finished')

    # write file
    init_str = fs_util.gen_str_by_repeat('a', 100)
    fs_util.write_file(fd, init_str)

    # close file before client b closes
    fs_util.close_file(fd)

    # send signal to client_b
    last_signal_name = cur_signal_name
    cur_signal_name = next(signal_name_gen)
    fs_util.send_signal(host_b, cur_signal_name)

    # time for client_b to work

    # open file
    fd = fs_util.open_file(FNAME)

    # read file
    cur_str = fs_util.read_file(fd, 100)
    assert len(cur_str) == 100

    # check if the content are all writen by client_b
    for idx, rc in enumerate(cur_str):
        if rc != 'b':
            print(f'Error idx:{idx} rc:{rc}')
    fs_util.close_file(fd)

    # done
    fs_util.record_test_result(TEST_CASE_NO, 'A', 'OK')


if __name__ == '__main__':
    run_test()
