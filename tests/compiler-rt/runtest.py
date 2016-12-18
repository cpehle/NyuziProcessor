#!/usr/bin/env python
#
# Copyright 2011-2015 Jeff Bush
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import sys
import subprocess
import os
from os import path

sys.path.insert(0, '..')
from test_harness import *


def run_emulator_test(source_file):
    build_program([source_file])
    result = run_program(environment='emulator')
    check_result(source_file, result)

register_tests(run_emulator_test, ['compiler-rt.c'])
execute_tests()
