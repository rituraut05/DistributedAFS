#! /usr/bin/env python3

import os
import fs_util
import sys
import time
'''
This is ClientA.
'''

cs739_env_vars = [
    'CS739_CLIENT_A', 'CS739_CLIENT_B', 'CS739_SERVER', 'CS739_MOUNT_POINT'
]
ENV_VARS = {var_name: os.environ.get(var_name) for var_name in cs739_env_vars}
for env_var in ENV_VARS.items():
    print(env_var)
    assert env_var is not None
TEST_DATA_DIR = ENV_VARS['CS739_MOUNT_POINT'] + '/test_consistency'
FNAME = f'{TEST_DATA_DIR}/case3'
print(TEST_DATA_DIR)
TEST_CASE_NO = 3


def run_test():

    if not fs_util.path_exists(TEST_DATA_DIR):
        fs_util.mkdir(TEST_DATA_DIR)

    # init
    if not fs_util.path_exists(FNAME):
        fs_util.create_file(FNAME)

    init_str = fs_util.gen_str_by_repeat('0', 200)
    fd = fs_util.open_file(FNAME)
    fs_util.write_file(fd, init_str)
    fs_util.close_file(fd)

    # open again
    fd = fs_util.open_file(FNAME)

    fs_util.kill_client("localhost")

    fs_util.record_test_result(TEST_CASE_NO, 'B', "Killed client with a opened file")

    
    # read the old content
    cur_str = fs_util.read_file(fd, 200)
    assert len(cur_str) == 200
    for idx, rc in enumerate(cur_str):
        if rc != '0':
            print(f'Error idx:{idx} rc:{rc}')
    fs_util.close_file(fd)

    fs_util.record_test_result(TEST_CASE_NO, 'B', "Closed a file after killed client")

    # read the things client_b wrote
    fd = fs_util.open_file(FNAME)
    cur_str = fs_util.read_file(fd, 200, start_off=0)
    assert len(cur_str) == 200

    for idx, c in enumerate(cur_str):
        assert c == '0'

    # now let's write again
    cur_str = fs_util.gen_str_by_repeat('a', 100)
    # use write here to see of FD's offset is correct
    fs_util.write_file(fd, cur_str)

    cur_str = fs_util.read_file(fd, 300)
    if len(cur_str) != 300:
        fs_util.record_test_result(TEST_CASE_NO, 'B',
                                   f'read_len:{len(cur_str)}')
    for idx, c in enumerate(cur_str):
        if idx < 100:
            if c != 'a':
                fs_util.record_test_result(TEST_CASE_NO, 'B',
                                           f'idx:{idx} c:{c}')
                sys.exit(1)
        elif idx < 200:
            if c != 'b':
                fs_util.record_test_result(TEST_CASE_NO, 'B',
                                           f'idx:{idx} c:{c}')
                sys.exit(1)
        else:
            if c != '0':
                fs_util.record_test_result(TEST_CASE_NO, 'B',
                                           f'idx:{idx} c:{c}')
                sys.exit(1)

    fs_util.close_file(fd)

    fs_util.record_test_result(TEST_CASE_NO, 'B', "Closed a file after updating with killed client")
    # done
    fs_util.record_test_result(TEST_CASE_NO, 'B', 'OK')


if __name__ == '__main__':
    run_test()
