#!/usr/bin/env python
#
#===- check_clang_tidy.py - ClangTidy Test Helper ------------*- python -*--===#
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===------------------------------------------------------------------------===#
# 
# This script is used for unit tests.
# Usage:
# ./check_clang_tidy.py <test_file_name>.cpp '<check_names>'
# Examples:
# ./check_clang_tidy.py test/aliceO2-aliceO2-namespace-naming.cpp 'aliceO2*'
# or
# ./check_clang_tidy.py test/aliceO2-aliceO2-namespace-naming.cpp 'aliceO2-namespace-naming'
#


r"""
ClangTidy Test Helper
=====================

This script runs clang-tidy in fix mode and verify fixes, messages or both.

Usage:
  check_clang_tidy.py [-resource-dir <resource-dir>] \
    <source-file> <check-name> \
    -- [optional clang-tidy arguments]

Example:
  // RUN: %check_clang_tidy %s llvm-include-order %t -- -- -isystem %S/Inputs
"""

import argparse
import re
import subprocess
import sys
import os


def write_file(file_name, text):
  with open(file_name, 'w') as f:
    f.write(text)
    f.truncate()
    
def get_configs(config_folder):
  configs = ''

  for root, dirs, files in os.walk(config_folder):
    for name in files: 
      with open(os.path.join(root, name), 'r') as f:
        for line in f:
          if configs:
            configs += ','
          configs += line.rstrip()
        
  return configs
  
def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-resource-dir')
  parser.add_argument('input_file_name')
  parser.add_argument('check_name')

  args, extra_args = parser.parse_known_args()

  resource_dir = args.resource_dir
  input_file_name = args.input_file_name
  check_name = args.check_name
  configs_dir = './configs/'

  extension = '.cpp'
  if (input_file_name.endswith('.c')):
    extension = '.c'
  if (input_file_name.endswith('.hpp')):
    extension = '.hpp'

  temp_file_name = input_file_name.replace(extension, '_result'+extension)

  clang_tidy_extra_args = extra_args
  if len(clang_tidy_extra_args) == 0:
    clang_tidy_extra_args = ['--', '--std=c++11'] if extension == '.cpp' \
                       else ['--']
  if resource_dir is not None:
    clang_tidy_extra_args.append('-resource-dir=%s' % resource_dir)

  with open(input_file_name, 'r') as input_file:
    input_text = input_file.read()

  has_check_fixes = input_text.find('CHECK-FIXES') >= 0
  has_check_messages = input_text.find('CHECK-MESSAGES') >= 0

  if not has_check_fixes and not has_check_messages:
    sys.exit('Neither CHECK-FIXES nor CHECK-MESSAGES found in the input')

  # Remove the contents of the CHECK lines to avoid CHECKs matching on
  # themselves.  We need to keep the comments to preserve line numbers while
  # avoiding empty lines which could potentially trigger formatting-related
  # checks.
  cleaned_test = re.sub('// *CHECK-[A-Z-]*:[^\r\n]*', '//', input_text)

  write_file(temp_file_name, cleaned_test)

  original_file_name = temp_file_name + ".orig"
  write_file(original_file_name, cleaned_test)

  configs = get_configs(configs_dir)
  if configs:
    args = ['O2codecheck', temp_file_name, '-fix', '-config={CheckOptions: ['+configs+']}', '--checks=-*,' + check_name] + \
          clang_tidy_extra_args
  else:
    args = ['O2codecheck', temp_file_name, '-fix', '--checks=-*,' + check_name] + \
          clang_tidy_extra_args
          
  print('Running ' + repr(args) + '...')
    
  try:
    clang_tidy_output = \
        subprocess.check_output(args, stderr=subprocess.STDOUT).decode()
  except subprocess.CalledProcessError as e:
    clang_tidy_output = e.output.decode()
    
    
  try:
    diff_output = subprocess.check_output(
        ['diff', '-u', original_file_name, temp_file_name],
        stderr=subprocess.STDOUT)
  except subprocess.CalledProcessError as e:
    diff_output = e.output

  if has_check_messages:
    messages_file = temp_file_name + '.msg'
    write_file(messages_file, clang_tidy_output)

  print('------------------------------ Fixes -----------------------------\n' +
        diff_output.decode() +
        '\n------------------------------------------------------------------')

  if has_check_fixes:
    try:
      subprocess.check_output(
          ['FileCheck', '-input-file=' + temp_file_name, input_file_name,
           '-check-prefix=CHECK-FIXES', '-strict-whitespace'],
          stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
      print('FileCheck failed:\n' + e.output.decode())
      raise

  if has_check_messages:
    #messages_file = temp_file_name + '.msg'
    #write_file(messages_file, clang_tidy_output)
    try:
      subprocess.check_output(
          ['FileCheck', '-input-file=' + messages_file, input_file_name,
           '-check-prefix=CHECK-MESSAGES',
           '-implicit-check-not={{warning|error}}:'],
          stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
      print('FileCheck failed:\n' + e.output.decode())
      raise
  
if __name__ == '__main__':
  main()
