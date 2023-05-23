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
FNAME = f'{TEST_DATA_DIR}/case8'
print(TEST_DATA_DIR)
TEST_CASE_NO = 8


def run_test():

    if not fs_util.path_exists(TEST_DATA_DIR):
        fs_util.mkdir(TEST_DATA_DIR)

    # init
    if not fs_util.path_exists(FNAME):
        fs_util.create_file(FNAME)

    # open
    fd = fs_util.open_file(FNAME)

    # write something
    cur_str = fs_util.gen_str_by_repeat('1', 100)
    fs_util.write_file(fd, cur_str)

    #close the file
    fs_util.close_file(fd)
    
    # open again
    fd = fs_util.open_file(FNAME)

    #write something
    cur_str = fs_util.gen_str_by_repeat('2', 100)
    fs_util.write_file(fd, cur_str)

    # kill client
    fs_util.kill_client("localhost")
    fs_util.record_test_result(TEST_CASE_NO, 'B', "Killed client with a opened file")

    #close the file
    ret = fs_util.close_file(fd)
    assert ret == -100
    fs_util.record_test_result(TEST_CASE_NO, 'B', "Closed a file after killed client")

    # bring client back
    fs_util.start_client("localhost")
    
    # open again
    fd = fs_util.open_file(FNAME)

    # read the things client_b wrote
    cur_str = fs_util.read_file(fd, 100)
    assert len(cur_str) == 100
    for idx, rc in enumerate(cur_str):
        if rc != '1':
            print(f'Error idx:{idx} rc:{rc}')
    fs_util.close_file(fd)
    # done
    fs_util.record_test_result(TEST_CASE_NO, 'B', 'OK')


if __name__ == '__main__':
    run_test()
